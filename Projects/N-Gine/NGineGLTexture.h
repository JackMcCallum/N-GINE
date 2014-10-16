#ifndef __NGINE_GL_TEXTURE_H_
#define __NGINE_GL_TEXTURE_H_

#include "NGineCommon.h"

namespace NGine
{
	class GLTexture2D;
	//class GLTexture2DArray;
	class GLTextureCube;
	//class GLTexture3D;

	class GLTexture
	{
	public:
		virtual ~GLTexture() {};

		// Get a textures type, returns nullptr if it is not of that type
		virtual GLTexture2D* getTexture2D() { return nullptr; }
		//virtual GLTexture2DArray* getTexture2DArray() { return nullptr; }
		virtual GLTextureCube* getTextureCube() { return nullptr; }
		//virtual GLTexture3D* getTexture3D() { return nullptr; }

	protected:
		
		static void _pixelFormatParams(PixelFormat format,
			GLenum& internalFormat,
			GLenum& lockFormat,
			GLenum& lockType);

		static size_t _bytesPerPixel(PixelFormat format);

		// Calculates a mip map size based on the mip level, mip level 0 is the base size and will return original
		static uint32 _mipSize(uint32 original, uint32 mip);

	};
}

#endif // !__NGINE_TEXTURE_H_
