#include "NGineStable.h"
#include "NGineGLMesh.h"
#include "NGineRenderCommand.h"

namespace NGine
{
	GLMesh::GLMesh()
	{
		mRenderData = RenderDataPtr(new RenderData);
		memset(mRenderData.get(), 0, sizeof(RenderData));
		memset(&mAttributes, 0, sizeof(Attributes));
	}

	GLMesh::~GLMesh()
	{
		ENQUEUE_RENDER_COMMAND_1PARAMS(GLMesh,
			RenderDataPtr, mRenderData, mRenderData,
			{
			if (mRenderData->vaoHandle)
				glDeleteVertexArrays(1, &mRenderData->vaoHandle);
			mRenderData->vaoHandle = 0;
			mRenderData->iboHandle = 0;
			mRenderData->vboHandle = 0;
		});
	}

	void GLMesh::draw(GLenum mode, GLenum type, uint32 offset, uint32 count, uint32 instances)
	{
		assert(instances != 0);

		DrawData drawData;
		drawData.count = count;
		drawData.offset = offset;
		drawData.mode = mode;
		drawData.type = type;
		drawData.instances = instances;

		// Update the attributes if necessary
		if (mAttributes.outOfDate)
		{
			mAttributes.outOfDate = false;
			ENQUEUE_RENDER_COMMAND_4PARAMS(GLMesh,
		 		GLMesh::RenderDataPtr, mRenderData, mRenderData,
				GLBuffer::RenderDataPtr, mVertexData, mVertexBuffer.mRenderData,
				GLBuffer::RenderDataPtr, mIndexData, mIndexBuffer.mRenderData,
				Attributes, mAttributes, mAttributes,
				{
				if (!mRenderData->vaoHandle)
					glGenVertexArrays(1, &mRenderData->vaoHandle);

				Main::getRenderSystem()._rtBindMesh(mRenderData->vaoHandle);

				// Bind element buffer if needed
				if (mRenderData->iboHandle != mIndexData->bufferHandle)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexData->bufferHandle);
					mRenderData->iboHandle = mIndexData->bufferHandle;
				}

				glBindBuffer(GL_ARRAY_BUFFER, mVertexData->bufferHandle);

				for (int i = 0; i < 8; i++)
				{
					if (mAttributes.attribute[i].enabled)
					{
						glEnableVertexAttribArray(i);
						glVertexAttribPointer(i,
							mAttributes.attribute[i].count, 
							mAttributes.attribute[i].type,
							mAttributes.attribute[i].normalized,
							mAttributes.attribute[i].stride,
							(char*)0 + mAttributes.attribute[i].offset);
					}
				}
			});
		}
		
		ENQUEUE_RENDER_COMMAND_4PARAMS(GLMesh,
			GLMesh::RenderDataPtr, mRenderData, mRenderData,
			GLBuffer::RenderDataPtr, mVertexData, mVertexBuffer.mRenderData,
			GLBuffer::RenderDataPtr, mIndexData, mIndexBuffer.mRenderData,
			DrawData, drawData, drawData,
			{

			if (!mRenderData->vaoHandle)
				glGenVertexArrays(1, &mRenderData->vaoHandle);

			Main::getRenderSystem()._rtBindMesh(mRenderData->vaoHandle);

			// Bind element buffer if needed
			if (mRenderData->iboHandle != mIndexData->bufferHandle)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexData->bufferHandle);
				mRenderData->iboHandle = mIndexData->bufferHandle;
			}

			if (!mRenderData->iboHandle)
			{
				// Draw vertices
				if (drawData.instances == 1)
					glDrawArrays(drawData.mode, drawData.offset, drawData.count);
				else
					glDrawArraysInstanced(drawData.mode, drawData.offset, drawData.count, drawData.instances);
			}
			else if (mRenderData->iboHandle)
			{
				// Draw elements
				// Work out the offset and type
				uint32 newoffset = 0;
				switch (drawData.type)
				{
				case GL_UNSIGNED_BYTE:
					newoffset = drawData.offset * sizeof(uint8);
					break;
				case GL_UNSIGNED_SHORT:
					newoffset = drawData.offset * sizeof(uint16);
					break;
				case GL_UNSIGNED_INT:
					newoffset = drawData.offset * sizeof(uint32);
					break;
				}

				if (drawData.instances == 1)
					glDrawElements(drawData.mode, drawData.count, drawData.type, (char*)newoffset);
				else
					glDrawElementsInstanced(drawData.mode, drawData.count, drawData.type, (char*)newoffset, drawData.instances);
			}
		});

	}

	void GLMesh::setAttribute(uint32 idx, bool enabled, uint32 count, GLenum type, uint32 offset, uint32 stride, bool normalized)
	{
		mAttributes.attribute[idx].enabled = enabled;
		mAttributes.attribute[idx].count = count;
		mAttributes.attribute[idx].type = type;
		mAttributes.attribute[idx].offset = offset;
		mAttributes.attribute[idx].stride = stride;
		mAttributes.attribute[idx].normalized = normalized;
		mAttributes.outOfDate = true;
	}

	GLBuffer& GLMesh::getVertexBuffer()
	{
		return mVertexBuffer;
	}

	const GLBuffer& GLMesh::getVertexBuffer() const
	{
		return mVertexBuffer;
	}

	GLBuffer& GLMesh::getIndexBuffer()
	{
		return mIndexBuffer;
	}

	const GLBuffer& GLMesh::getIndexBuffer() const
	{
		return mIndexBuffer;
	}

}