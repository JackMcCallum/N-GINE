#include "NGineStable.h"
#include "NGineImageFile.h"

namespace NGine
{
	ImageFile::ImageFile() :
		mPixelFormat(PF_UBYTE_R),
		mWidth(0),
		mHeight(0),
		mLevels(0),
		mIsLoaded(false),
		mBytesPerPixel(0)
	{
		for (uint8 i = 0; i < 16; i++)
			mImageData[i] = nullptr;
	}

	ImageFile::ImageFile(const std::string& fname) :
		mPixelFormat(PF_UBYTE_R),
		mWidth(0),
		mHeight(0),
		mLevels(0),
		mIsLoaded(false),
		mBytesPerPixel(0)
	{
		for (uint8 i = 0; i < 16; i++)
			mImageData[i] = nullptr;

		load(fname);
	}

	ImageFile::~ImageFile()
	{
		for (uint8 i = 0; i < 16; i++)
			_DELETE_ARRAY_IF(mImageData[i]);
	}

	bool ImageFile::load(const std::string& fname)
	{
		FREE_IMAGE_FORMAT fiFormat = FreeImage_GetFileType(fname.c_str(), 0);
		if (fiFormat == FIF_UNKNOWN) return false;
		
		FIBITMAP* image = FreeImage_Load(fiFormat, fname.c_str());
		if (!image) return false;

		if (FreeImage_GetBPP(image) != 32)
		{
			FIBITMAP* convertedImage = FreeImage_ConvertTo32Bits(image);
			FreeImage_Unload(image);
			image = convertedImage;
		}

		mBytesPerPixel = FreeImage_GetBPP(image) / 8;
		
		mPixelFormat = PF_UBYTE_RGBA;
		mWidth = FreeImage_GetWidth(image);
		mHeight = FreeImage_GetHeight(image);
		mLevels = 1;

		for (uint8 i = 0; i < 16; i++)
			_DELETE_ARRAY_IF(mImageData[i]);

		// Allocate 1 level
		uint8* destData = new uint8[mWidth * mHeight * mBytesPerPixel];
		mImageData[0] = destData;

		uint8* srcData = FreeImage_GetBits(image);

		for (uint32 i = 0; i < mWidth; i++)
		{
			for (uint32 j = 0; j < mHeight; j++)
			{
				uint32 s = j * 4 * mWidth + i * 4;
				destData[s + 0] = srcData[s + 2]; // R, B
				destData[s + 1] = srcData[s + 1]; // G, G
				destData[s + 2] = srcData[s + 0]; // B, R
				destData[s + 3] = srcData[s + 3]; // A, A
			}
		}
		
		FreeImage_Unload(image);
		mIsLoaded = true;
	}

	PixelFormat ImageFile::getPixelFormat() const
	{
		return mPixelFormat;
	}

	uint16 ImageFile::getWidth(uint8 level) const
	{
		assert(level < mLevels);
		uint16 v = mWidth;
		for (uint8 i = 0; i < level; i++) v /= 2;
		return v;
	}

	uint16 ImageFile::getHeight(uint8 level) const
	{
		assert(level < mLevels);
		uint16 v = mHeight;
		for (uint8 i = 0; i < level; i++) v /= 2;
		return v;
	}

	uint8 ImageFile::getLevels() const
	{
		return mLevels;
	}

	void* ImageFile::getData(uint8 level) const
	{
		assert(level < mLevels);
		return mImageData[level];
	}

	bool ImageFile::isLoaded() const
	{
		return mIsLoaded;
	}

	uint8 ImageFile::getBytesPerPixel() const
	{
		return mBytesPerPixel;
	}

}