#include "NGineStable.h"
#include "NGineRenderTarget.h"
#include "NGineRenderSystem.h"
#include "NGineResourceManager.h"
#include "NGineCamera.h"
#include "NGineSceneNode.h"
#include "NGineComponent.h"
#include "NGineMain.h"
#include "NGineGizmos.h"
#include "NGineMemoryBlock.h"

namespace NGine
{

	RenderTarget::RenderTarget() :
		mWidth(0),
		mHeight(0),
		mCamera(nullptr)
	{
	}

	RenderTarget::~RenderTarget()
	{
	}

	void RenderTarget::updateTarget()
	{
		setActive();

		if (!_readyDrawCalls())
			return;

		_iterateDrawcalls(mCachedDrawCallList.begin(), mCachedDrawCallList.end());
	}

	void RenderTarget::setDimensions(uint32 width, uint32 height)
	{
		mWidth = width;
		mHeight = height;
	}

	void RenderTarget::setCamera(Camera* camera)
	{
		mCamera = camera;
	}

	bool RenderTarget::_readyDrawCalls()
	{
		mTriangleCount = 0;
		mBatchCount = 0;
		mSavedByBatching = 0;

		// If theres no camera, nothing to render
		if (!mCamera) return false;

		// Get the cameras parent, if its not attached to anything, theres nothing to render
		SceneNode* camParent = mCamera->getParent();
		if (!camParent) return false;

		// Find the highest node in the hierarchy
		SceneNode* rootNode = camParent->getRootNode();
		if (!rootNode) return false;

		Main::getRenderSystem().clear(glm::vec4(0.1,0.1,0.1, 1), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mCachedRenderableList.clear();
		rootNode->_getRenderables(mCachedRenderableList, mCamera, true);

		mCachedDrawCallList.clear();
		for (auto iter : mCachedRenderableList)
			iter->_extractDrawCalls(mCachedDrawCallList, *mCamera);

		std::sort(mCachedDrawCallList.begin(), mCachedDrawCallList.end(),
			[](const Renderable::DrawCall& first, const Renderable::DrawCall& second)
		{
			return first.sortKey.key < second.sortKey.key;
		});

		// Update the render system with the camera data
		glm::mat4 camMatrix = mCamera->getParent()->getGlobalTransform();
		glm::mat4 viewMatrix = glm::inverse(camMatrix);
		ngRenderSys.setCameraMatrices(mCamera->getProjMatrix(), viewMatrix, camMatrix);



		return true;
	}

	void RenderTarget::_iterateDrawcalls(DrawCallIterator start, DrawCallIterator end)
	{
		if (start == mCachedDrawCallList.end())
			return;

		mDrawCallIterator = start;

		uint32 batchCount = 0;
		MaterialPtr lastMaterial;
		mDrawCallIterator = start;
		Renderable::DrawCall& batchedDrawCall = *mDrawCallIterator;

		bool flushBatch = false;
		while (mDrawCallIterator != end)
		{
			Renderable::DrawCall& drawCall = *mDrawCallIterator;

			if (drawCall == batchedDrawCall && batchCount < 512)
			{
				// Update the world matrix
				if (drawCall.parent)
					//ngRenderSys.setWorldMatrix(drawCall.parent->getGlobalTransform(), batchCount++);
					mWorldMatrixCache[batchCount++] = drawCall.parent->getGlobalTransform();
				
				if (++mDrawCallIterator == end)
					flushBatch = true;
			}
			else
				flushBatch = true;

			if (flushBatch && batchCount > 0)
			{
				MemoryBlock memoryBlock(sizeof(glm::mat4) * batchCount);
				memcpy(memoryBlock.data(), mWorldMatrixCache, sizeof(glm::mat4) * batchCount);
				ngRenderSys.setWorldMatrices(memoryBlock, batchCount, 0);

				//Main::getRenderSystem().setWorldMatrices(mWorldMatrixCache, batchCount, 0);

				if (batchedDrawCall.material != lastMaterial)
				{
					lastMaterial = batchedDrawCall.material;
					if (batchedDrawCall.material)
						batchedDrawCall.material->setActive();
				}

				// Make the draw call
				batchedDrawCall.glMesh->draw(
					batchedDrawCall.drawMode,
					batchedDrawCall.indexType,
					batchedDrawCall.offset,
					batchedDrawCall.count,
					batchCount);

				mTriangleCount += (batchedDrawCall.count / 3) * batchCount;
				mSavedByBatching += batchCount - 1;
				mBatchCount += 1;

				// Reset the batch counter
				if (mDrawCallIterator != end)
					batchedDrawCall = *mDrawCallIterator;

				batchCount = 0;
				flushBatch = false;
			}
		}
	}




}