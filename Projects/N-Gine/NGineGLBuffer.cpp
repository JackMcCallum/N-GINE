#include "NGineStable.h"
#include "NGineGLBuffer.h"
#include "NGineRenderCommand.h"

namespace NGine
{
	GLBuffer::GLBuffer(size_t size, BufferUsage usage) :
		mInitialized(false),
		mSize(size),
		mUsage(usage),
		mLockDeleteAfter(false),
		mLockData(nullptr),
		mLockOffset(0),
		mLockLength(0)
	{
		mRenderData = RenderDataPtr(new RenderData);
		mRenderData->bufferHandle = 0;
	}

	GLBuffer::~GLBuffer()
	{
		_release();
	}

	void GLBuffer::_init()
	{
		if (!mInitialized)
		{
			mInitialized = true;
			ENQUEUE_RENDER_COMMAND_3PARAMS(GLBuffer,
				RenderDataPtr, mRenderData, mRenderData,
				size_t, mSize, mSize,
				BufferUsage, mUsage, mUsage,
				{
				glGenBuffers(1, &mRenderData->bufferHandle);
				if (mSize > 0)
				{
					Main::getRenderSystem()._rtBindCopyWriteBuffer(mRenderData->bufferHandle);
					glBufferData(GL_COPY_WRITE_BUFFER, mSize, nullptr, _translateUseage(mUsage));
				}
			});
		}
	}

	void GLBuffer::_release()
	{
		if (mInitialized)
		{
			mInitialized = false;
			ENQUEUE_RENDER_COMMAND_1PARAMS(GLBuffer,
				RenderDataPtr, mRenderData, mRenderData,
				{
				if (mRenderData->bufferHandle)
				{
					glDeleteBuffers(1, &mRenderData->bufferHandle);
					mRenderData->bufferHandle = 0;
				}
			});
		}
	}

	void* GLBuffer::lock(size_t offset, size_t length)
	{
		assert((offset + length) <= mSize);
		assert(!(length == 0 && offset != 0));

		mLockOffset = offset;
		mLockLength = (length == 0 && offset == 0) ? mSize : length;

		// Try to allocate from the scratch pad but if not, new the memory from the heap
		mLockData = Main::getRenderSystem().allocateScratch(mLockLength);
		if (mLockData)
			mLockDeleteAfter = false;
		else
		{
			mLockData = new uint8[mLockLength];
			mLockDeleteAfter = true;
		}
		return mLockData;
	}

	void GLBuffer::unlock()
	{
		_init();

		if (mLockData)
		{
			ENQUEUE_RENDER_COMMAND_5PARAMS(GLBuffer,
				RenderDataPtr, mRenderData, mRenderData,
				void*, mLockData, mLockData,
				size_t, mLockOffset, mLockOffset,
				size_t, mLockLength, mLockLength,
				bool, mLockDeleteAfter, mLockDeleteAfter,
				{
				Main::getRenderSystem()._rtBindCopyWriteBuffer(mRenderData->bufferHandle);
				glBufferSubData(GL_COPY_WRITE_BUFFER, mLockOffset, mLockLength, mLockData);

				if (mLockDeleteAfter)
					delete[] mLockData;
			});
		}
	}

	void GLBuffer::resize(size_t size, BufferUsage usage)
	{
		mUsage = usage;
		mSize = size;

		if (mInitialized)
		{
			ENQUEUE_RENDER_COMMAND_3PARAMS(GLBuffer,
				RenderDataPtr, mRenderData, mRenderData,
				size_t, mSize, mSize,
				BufferUsage, mUsage, mUsage,
				{
				if (mSize > 0)
				{
					Main::getRenderSystem()._rtBindCopyWriteBuffer(mRenderData->bufferHandle);
					glBufferData(GL_COPY_WRITE_BUFFER, mSize, nullptr, _translateUseage(mUsage));
				}
			});
		}
	}

	GLenum GLBuffer::_translateUseage(BufferUsage usage)
	{
		switch (usage)
		{
		case BU_STATIC: return GL_STATIC_DRAW; break;
		case BU_DYNAMIC: return GL_DYNAMIC_DRAW; break;
		case BU_STREAM: return GL_STREAM_DRAW; break;
		}
		return GL_STATIC_DRAW;
	}

	size_t GLBuffer::getSize()
	{
		return mSize;
	}

}