#include "NGineStable.h"
#include "NGineGLFrameBuffer.h"
#include "NGineRenderCommand.h"
#include "NGineGLTexture2D.h"

namespace NGine
{
	GLFrameBuffer::GLFrameBuffer() :
		mInitialized(false)
	{
		mRenderData = RenderDataPtr(new RenderData);
		mRenderData->frameBufferHandle = 0;


	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		_release();
	}

	void GLFrameBuffer::attachGLTexture2D(GLTexture2D* texture, GLenum attachment)
	{
		assert(texture);

		_init();
		ENQUEUE_RENDER_COMMAND_3PARAMS(GLFrameBuffer,
			RenderDataPtr, mRenderData, mRenderData,
			GLenum, attachment, attachment,
			GLTexture2D::RenderDataPtr, mTexData, texture->mRenderData,
			{
			Main::getRenderSystem()._rtBindFrameBuffer(mRenderData->frameBufferHandle);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, mTexData->texHandle, 0);
		});
	}

	GLenum GLFrameBuffer::getStatus()
	{
		_init();

		GLenum status;
		ENQUEUE_RENDER_COMMAND_2PARAMS(GLFrameBuffer,
			RenderDataPtr, mRenderData, mRenderData,
			GLenum*, status, &status,
			{
			Main::getRenderSystem()._rtBindFrameBuffer(mRenderData->frameBufferHandle);
			*status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		});
		Main::getRenderSystem().flushQueue();
		return status;
	}

	void GLFrameBuffer::_init()
	{
		if (!mInitialized)
		{
			mInitialized = true;
			ENQUEUE_RENDER_COMMAND_1PARAMS(GLFrameBuffer,
				RenderDataPtr, mRenderData, mRenderData,
				{
				if (!mRenderData->frameBufferHandle)
					glGenFramebuffers(1, &mRenderData->frameBufferHandle);
			});
		}
	}

	void GLFrameBuffer::_release()
	{
		if (mInitialized)
		{
			mInitialized = false;
			ENQUEUE_RENDER_COMMAND_1PARAMS(GLFrameBuffer,
				RenderDataPtr, mRenderData, mRenderData,
				{
				if (!mRenderData->frameBufferHandle)
					glDeleteFramebuffers(1, &mRenderData->frameBufferHandle);
				mRenderData->frameBufferHandle = 0;
			});
		}
	}

}