#include "NGineStable.h"
#include "NGineTexture.h"
#include "NGineGLTexture2D.h"
#include "NGineImageFile.h"

namespace NGine
{
	Texture::Texture(const std::string& name, uint32 width, uint32 height, PixelFormat format, uint32 mipLevels) :
		Resource(name),
		mGLTexture(nullptr),
		mTextureType(TEXTURE_2D),
		mManual(true)
	{
		mGLTexture = new GLTexture2D(width, height, format, mipLevels);

		for (int i = 0; i < 6; i++)
			mImageFile[i] = nullptr;
	}

	Texture::Texture(const std::string& name, const std::string fname) :
		Resource(name),
		mGLTexture(nullptr),
		mTextureType(TEXTURE_2D),
		mManual(false),
		mFileName(fname)
	{
		for (int i = 0; i < 6; i++)
			mImageFile[i] = nullptr;
	}

	Texture::Texture(const std::string& name, const std::string fname, const std::string& ending) :
		Resource(name),
		mGLTexture(nullptr),
		mTextureType(TEXTURE_2D),
		mManual(false),
		mFileName(fname),
		mEndings(ending)
	{
		for (int i = 0; i < 6; i++)
			mImageFile[i] = nullptr;
	}

	Texture::~Texture()
	{
		_DELETE_IF(mGLTexture);
		
		for (int i = 0; i < 6; i++)
			_DELETE_IF(mImageFile[i]);
	}

	GLTexture* Texture::getGLTexture()
	{
		return mGLTexture;
	}

	bool Texture::prepare()
	{
		if (mManual) return false;
		if (mPrepared) return true;

		if (mFileName.empty())
			return false;

		if (mTextureType == TEXTURE_2D)
		{
			mImageFile[0] = new ImageFile();
			if (!mImageFile[0]->load(mFileName))
			{
				_DELETE_IF(mImageFile[0]);
				return false;
			}
		}
		else if (mTextureType == TEXTURE_CUBE)
		{
			std::string dir, name, ext, token;
			std::stringstream endings(mEndings);

			for (int i = 0; i < 6; i++)
			{
				Utilities::splitPath(mFileName, &dir, &name, &ext);

				endings >> token;
				std::string fname = dir + name + token + ext;

				mImageFile[i] = new ImageFile();
				if (!mImageFile[i]->load(mFileName))
				{
					for (int i = 0; i < 6; i++)
						_DELETE_IF(mImageFile[i]);
					return false;
				}
			}
		}

		mPrepared = true;
		return true;
	}

	bool Texture::unprepare()
	{
		if (mManual) return false;
		if (!mPrepared) return false;

		for (int i = 0; i < 6; i++)
			_DELETE_IF(mImageFile[i]);

		mPrepared = false;
		return true;
	}

	bool Texture::load()
	{
		if (mManual) return false;
		if (mLoaded) return true;

		if (mTextureType == TEXTURE_2D)
		{
			if (!prepare())
				return false;

			GLTexture2D* texture = new GLTexture2D(
				mImageFile[0]->getWidth(), 
				mImageFile[0]->getHeight(),
				mImageFile[0]->getPixelFormat(),
				0); // 0 Mip levels inicates automatic

			void* data = texture->lock(0,0,0,0,0,true);
			if (data)
			{
				size_t size = mImageFile[0]->getWidth(0) * mImageFile[0]->getHeight(0) * mImageFile[0]->getBytesPerPixel();
				memcpy(data, mImageFile[0]->getData(), size);
				texture->unlock();
			}
			ngRenderSys.flushQueue();

			unprepare();

			mGLTexture = texture;
		}
		else if (mTextureType == TEXTURE_CUBE)
		{
			if (!prepare())
				return false;

			assert(false);
		}

		mLoaded = true;
		return true;
	}

	bool Texture::unload()
	{
		if (mManual) return false;
		if (!mLoaded) return false;

		_DELETE_IF(mGLTexture);

		mLoaded = false;
		return true;
	}

}