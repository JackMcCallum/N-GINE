#include "NGineStable.h"
#include "NGineGLTexture2D.h"
#include "NGineRenderCommand.h"

namespace NGine
{
	GLTexture2D::GLTexture2D(uint16 width, uint16 height, PixelFormat format, uint8 mips)
	{
		mRenderData = RenderDataPtr(new RenderData);
		mRenderData->texHandle = 0;

		mTexData.width = width;
		mTexData.height = height;

		if (mips == 0)
		{
			uint32 m = 1;
			uint32 s = mTexData.width > mTexData.height ? mTexData.width : mTexData.height;
			while (s > 1)
			{
				s /= 2;
				m++;
			}
			mTexData.mipLevels = m;
		}
		else
		{
			mTexData.mipLevels = mips;
		}
		
		mTexData.pixelFormat = format;
		mIsLocked = false;

		ENQUEUE_RENDER_COMMAND_2PARAMS(GLTexture2D,
			RenderDataPtr, mRenderData, mRenderData,
			TexData, mTexData, mTexData,
			{
			if (!mRenderData->texHandle)
			{
				GLenum internalFormat;
				GLenum lockFormat;
				GLenum lockType;
				GLTexture::_pixelFormatParams(mTexData.pixelFormat, internalFormat, lockFormat, lockType);

				glGenTextures(1, &mRenderData->texHandle);
				ngRenderSys._rtBindTexture2D(mRenderData->texHandle, 0);
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mTexData.width, mTexData.height, 0, lockFormat, lockType, nullptr);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mTexData.mipLevels - 1);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				
				if (mTexData.mipLevels > 1)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
		});
	}

	GLTexture2D::~GLTexture2D()
	{
		ENQUEUE_RENDER_COMMAND_1PARAMS(GLTexture2D,
			RenderDataPtr, mRenderData, mRenderData,
			{
			if (mRenderData->texHandle)
			{
				glDeleteTextures(1, &mRenderData->texHandle);
				mRenderData->texHandle = 0;
			}
		});
	}

	uint16 GLTexture2D::getWidth(uint8 level) const
	{
		assert(level < mTexData.mipLevels);
		uint16 v = mTexData.width;
		for (uint8 i = 0; i < level; i++) v /= 2;
		return v;
	}

	uint16 GLTexture2D::getHeight(uint8 level) const
	{
		assert(level < mTexData.mipLevels);
		uint16 v = mTexData.height;
		for (uint8 i = 0; i < level; i++) v /= 2;
		return v;
	}

	uint8 GLTexture2D::getNumMips() const
	{
		return mTexData.mipLevels;
	}

	NGine::PixelFormat GLTexture2D::getPixelFormat() const
	{
		return mTexData.pixelFormat;
	}

	void* GLTexture2D::lock(uint16 x, uint16 y, uint16 w, uint16 h, uint8 level, bool genMips)
	{
		assert(!mIsLocked);
		if (mIsLocked)
			return nullptr;

		mLockData.x = x;
		mLockData.y = y;
		mLockData.w = w == 0 ? getWidth(level) : w;
		mLockData.h = h == 0 ? getHeight(level) : h;
		mLockData.level = level;
		mLockData.genMips = genMips;

		assert(mLockData.w + mLockData.x <= getWidth(level));
		assert(mLockData.h + mLockData.y <= getHeight(level));

		// Allocate the data we will send back to the user to fill
		mLockData.data = MemoryBlock(_bytesPerPixel(mTexData.pixelFormat) * mLockData.w * mLockData.h);

		mIsLocked = true;
		return mLockData.data.data();
	}

	void GLTexture2D::unlock()
	{
		assert(mIsLocked);
		if (!mIsLocked)
			return;

		ENQUEUE_RENDER_COMMAND_3PARAMS(Upload,
			RenderDataPtr, mRenderData, mRenderData,
			TexData, mTexData, mTexData,
			LockData, mLockData, mLockData,
			{
			GLenum internalFormat;
			GLenum lockFormat;
			GLenum lockType;
			GLTexture::_pixelFormatParams(mTexData.pixelFormat, internalFormat, lockFormat, lockType);

			Main::getRenderSystem()._rtBindTexture2D(mRenderData->texHandle, 0);
			glTexSubImage2D(GL_TEXTURE_2D, mLockData.level,
				mLockData.x, mLockData.y, mLockData.w, mLockData.h,
				lockFormat, lockType, mLockData.data.data());

			mLockData.data.free();

			if (mLockData.genMips)
				glGenerateMipmap(GL_TEXTURE_2D);
		});

		mIsLocked = false;
	}

}

