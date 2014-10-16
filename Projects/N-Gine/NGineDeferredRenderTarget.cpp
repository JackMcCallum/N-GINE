#include "NGineStable.h"
#include "NGineDeferredRenderTarget.h"
#include "NGineGLFrameBuffer.h"
#include "NGineMain.h"
#include "NGineRenderCommand.h"
#include "NGineTexture.h"
#include "NGineMaterial.h"
#include "NGineLight.h"

namespace NGine
{

	DeferredRenderTarget::DeferredRenderTarget(uint32 width, uint32 height) :
		mGBuffer(nullptr),
		mHDRBuffer(nullptr)
	{
		setDimensions(width, height);

		// Create the textures we need
		mNormal = ngResourceMgr.createTexture("DRT_Normal", mWidth, mHeight, PF_FLOAT_RGB);
		mAlbedoAO = ngResourceMgr.createTexture("DRT_AlbedoAO", mWidth, mHeight, PF_UBYTE_RGBA);
		mEmissive = ngResourceMgr.createTexture("DRT_Emissive", mWidth, mHeight, PF_UBYTE_RGB);
		mSurface = ngResourceMgr.createTexture("DRT_Surface", mWidth, mHeight, PF_UBYTE_RG);
		mHDRTexture = ngResourceMgr.createTexture("DRT_HDRTexture", mWidth, mHeight, PF_FLOAT_RGB);
		mDepth24 = ngResourceMgr.createTexture("DRT_Depth24", mWidth, mHeight, PF_DEPTH_24);

		GLenum status = 0;
		mGBuffer = new GLFrameBuffer();
		mGBuffer->attachGLTexture2D((GLTexture2D*)mAlbedoAO->getGLTexture(), GL_COLOR_ATTACHMENT0);
		mGBuffer->attachGLTexture2D((GLTexture2D*)mNormal->getGLTexture(), GL_COLOR_ATTACHMENT1);
		mGBuffer->attachGLTexture2D((GLTexture2D*)mEmissive->getGLTexture(), GL_COLOR_ATTACHMENT2);
		mGBuffer->attachGLTexture2D((GLTexture2D*)mSurface->getGLTexture(), GL_COLOR_ATTACHMENT3);
		mGBuffer->attachGLTexture2D((GLTexture2D*)mDepth24->getGLTexture(), GL_DEPTH_ATTACHMENT);
		if (status = mGBuffer->getStatus() != GL_FRAMEBUFFER_COMPLETE)
			Log::error("GBuffer validation failed, code %i was returned", status);
		
		ENQUEUE_RENDER_COMMAND_0PARAMS(DrawBuffers,
			{
			GLenum buffers[4];
			buffers[0] = GL_COLOR_ATTACHMENT0;
			buffers[1] = GL_COLOR_ATTACHMENT1;
			buffers[2] = GL_COLOR_ATTACHMENT2;
			buffers[3] = GL_COLOR_ATTACHMENT3;
			glDrawBuffers(4, buffers);
		});

		mHDRBuffer = new GLFrameBuffer();
		mHDRBuffer->attachGLTexture2D((GLTexture2D*)mHDRTexture->getGLTexture(), GL_COLOR_ATTACHMENT0);
		mHDRBuffer->attachGLTexture2D((GLTexture2D*)mDepth24->getGLTexture(), GL_DEPTH_ATTACHMENT);
		if (status = mHDRBuffer->getStatus() != GL_FRAMEBUFFER_COMPLETE)
			Log::error("HDR Buffer validation failed, code %i was returned", status);
		
		ENQUEUE_RENDER_COMMAND_0PARAMS(DrawBuffers,
			{
			GLenum buffers = GL_COLOR_ATTACHMENT0;
			glDrawBuffers(1, &buffers);
		});

		mMatAmbientPass = ngResourceMgr.createMaterial("DRT_AmbientPass");
		mMatAmbientPass->setShader("S_DRT_AmbientPass.shader");
		mMatAmbientPass->setTexture(mAlbedoAO, 0);
		mMatAmbientPass->setTexture(mNormal, 1);
		mMatAmbientPass->setTexture(mEmissive, 2);
		mMatAmbientPass->setTexture(mSurface, 3);
		mMatAmbientPass->setDepthMode(false, false);

		mMatDirLight = ngResourceMgr.createMaterial("DRT_DirLight");
		mMatDirLight->setShader("S_DRT_Light_Directional.shader");
		mMatDirLight->setTexture(mAlbedoAO, 0);
		mMatDirLight->setTexture(mNormal, 1);
		mMatDirLight->setTexture(mSurface, 3);
		mMatDirLight->setTexture(mDepth24, 5);
		mMatDirLight->setDepthMode(false, false);
		mMatDirLight->setBlendMode(BM_ADDITIVE);

		mMatPointLight = ngResourceMgr.createMaterial("DRT_PointLight");
		mMatPointLight->setShader("S_DRT_Light_Point.shader");
		mMatPointLight->setTexture(mAlbedoAO, 0);
		mMatPointLight->setTexture(mNormal, 1);
		mMatPointLight->setTexture(mSurface, 3);
		mMatPointLight->setDepthMode(false, false);
		mMatPointLight->setBlendMode(BM_ADDITIVE);

	}

	DeferredRenderTarget::~DeferredRenderTarget()
	{
		delete mGBuffer;
		delete mHDRBuffer;
	}

	void DeferredRenderTarget::updateTarget()
	{
		// Ready all the draw calls
		if (!_readyDrawCalls())
			return;

		ngRenderSys.bindFrameBuffer(mGBuffer);
		ngRenderSys.setViewportMode(glm::vec4(0, 0, mWidth, mHeight));
		ngRenderSys.clear(glm::vec4(0, 0, 0, 0), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Find the iterator to the first transparent draw call, we dont want to include them in the GBuffer
		auto transparentStart = std::find_if(mCachedDrawCallList.begin(), mCachedDrawCallList.end(),
			[](const Renderable::DrawCall& drawCall)
		{
			return (drawCall.sortKey.transparent.layer == RL_TRANSPARENT);
		});

		// Render opaque geometry to the gbuffer
		_iterateDrawcalls(mCachedDrawCallList.begin(), transparentStart);

		ngRenderSys.bindFrameBuffer(mHDRBuffer);
		ngRenderSys.renderQuad(mMatAmbientPass);

		// Find the iterator to the first transparent draw call, we dont want to include them in the GBuffer
		auto guiStart = std::find_if(mCachedDrawCallList.begin(), mCachedDrawCallList.end(),
			[](const Renderable::DrawCall& drawCall)
		{
			return (drawCall.sortKey.transparent.layer == RL_GUI);
		});

		// Render all the lights onto the opaque geometry
		_lightPass();

		// Draw transparent objects
		_iterateDrawcalls(transparentStart, mCachedDrawCallList.end());

		// Post process
		//_postProcess();

		// Draw the GUI
		//_iterateDrawcalls(guiStart, mCachedDrawCallList.end());
	}

	void DeferredRenderTarget::_lightPass()
	{
		for (auto iter : mCachedRenderableList)
		{
			if (iter->getTypeInfo() == &typeid(PointLight))
			{

			}
			else if (iter->getTypeInfo() == &typeid(DirectionalLight))
			{
				DirectionalLight* light = (DirectionalLight*)iter;
				ngRenderSys.setCustomParams(glm::vec4(light->getDirection(), 1), glm::vec4(light->getColor(), 1));
				ngRenderSys.renderQuad(mMatDirLight);
			}
		}
	}

	void DeferredRenderTarget::_postProcess()
	{

	}
	
	void DeferredRenderTarget::setActive()
	{
		Main::getRenderSystem().bindFrameBuffer(mGBuffer);
		Main::getRenderSystem().setViewportMode(glm::vec4(0, 0, mWidth, mHeight));

	}


}