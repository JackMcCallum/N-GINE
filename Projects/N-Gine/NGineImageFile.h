#ifndef __NGINE_IMAGE_FILE_H_
#define __NGINE_IMAGE_FILE_H_

#include "NGineCommon.h"

namespace NGine
{
	class ImageFile
	{
	public:
		ImageFile();
		ImageFile(const std::string& fname);
		~ImageFile();

		bool load(const std::string& fname);

		PixelFormat getPixelFormat() const;

		uint16 getWidth(uint8 level = 0) const;
		uint16 getHeight(uint8 level = 0) const;

		uint8 getLevels() const;
		uint8 getBytesPerPixel() const;

		void* getData(uint8 level = 0) const;

		bool isLoaded() const;


	private:
		PixelFormat mPixelFormat;
		uint16 mWidth;
		uint16 mHeight;
		uint8 mLevels;
		uint8 mBytesPerPixel;
		
		bool mIsLoaded;
		void* mImageData[16];
	};
}

#endif // !__NGINE_IMAGE_FILE_H_
