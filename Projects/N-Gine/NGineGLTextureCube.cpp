#include "NGineStable.h"
#include "NGineGLTextureCube.h"
#include "NGineRenderCommand.h"

namespace NGine
{
	GLTextureCube::GLTextureCube(uint16 width, uint16 height, PixelFormat format, uint8 mips)
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
				Main::getRenderSystem()._rtBindTextureCube(mRenderData->texHandle, 0);
				
				for (int i = 0; i < 6; i++)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, 
					mTexData.width, mTexData.height, 0, lockFormat, lockType, nullptr);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, mTexData.mipLevels - 1);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);

				if (mTexData.mipLevels > 1)
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				}
			}
		});
	}

	GLTextureCube::~GLTextureCube()
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

	void* GLTextureCube::lock(uint32 face, uint16 x, uint16 y, uint16 w, uint16 h, uint8 level, bool genMips)
	{
		assert(!mIsLocked);
		assert(face < 6);

		if (mIsLocked)
			return nullptr;

		mLockData.x = x;
		mLockData.y = y;
		mLockData.w = w == 0 ? getWidth(level) : w;
		mLockData.h = h == 0 ? getHeight(level) : h;
		mLockData.level = level;
		mLockData.genMips = genMips;
		mLockData.face = face;

		assert(mLockData.w + mLockData.x <= getWidth(level));
		assert(mLockData.h + mLockData.y <= getHeight(level));

		// Allocate the data we will send back to the user to fill
		mLockData.data = ngRenderSys.allocateScratch(_bytesPerPixel(mTexData.pixelFormat) * mLockData.w * mLockData.h);

		if (mLockData.data)
			mIsLocked = true;

		return mLockData.data;
	}

	void GLTextureCube::unlock()
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

			Main::getRenderSystem()._rtBindTextureCube(mRenderData->texHandle, 0);

			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + mLockData.face, mLockData.level,
				mLockData.x, mLockData.y, mLockData.w, mLockData.h,
				lockFormat, lockType, mLockData.data);

			if (mLockData.genMips)
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		});

		mIsLocked = false;
	}

	uint16 GLTextureCube::getWidth(uint8 level) const
	{
		return mTexData.width;
	}

	uint16 GLTextureCube::getHeight(uint8 level) const
	{
		return mTexData.height;
	}

	uint8 GLTextureCube::getNumMips() const
	{
		return mTexData.mipLevels;
	}

	PixelFormat GLTextureCube::getPixelFormat() const
	{
		return mTexData.pixelFormat;
	}

}

