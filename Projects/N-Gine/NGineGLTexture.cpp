#include "NGineStable.h"
#include "NGineGLTexture.h"

namespace NGine
{
	uint32 GLTexture::_mipSize(uint32 original, uint32 mip)
	{
		uint32 size = original;
		for (int i = 0; i < mip; i++)
			size /= 2;
		return size;
	}

	void GLTexture::_pixelFormatParams(PixelFormat format, GLenum& internalFormat, GLenum& lockFormat, GLenum& lockType)
	{
		switch (format)
		{
		case PF_UBYTE_R:
			internalFormat = GL_R;
			lockFormat = GL_R;
			lockType = GL_UNSIGNED_BYTE;
			break;

		case PF_UBYTE_RG:
			internalFormat = GL_RG;
			lockFormat = GL_RG;
			lockType = GL_UNSIGNED_BYTE;
			break;

		case PF_UBYTE_RGB:
			internalFormat = GL_RGB;
			lockFormat = GL_RGB;
			lockType = GL_UNSIGNED_BYTE;
			break;

		case PF_UBYTE_RGBA:
			internalFormat = GL_RGBA;
			lockFormat = GL_RGBA;
			lockType = GL_UNSIGNED_BYTE;
			break;

		case PF_FLOAT_R:
			internalFormat = GL_R32F;
			lockFormat = GL_R;
			lockType = GL_FLOAT;
			break;

		case PF_FLOAT_RG:
			internalFormat = GL_RG32F;
			lockFormat = GL_RG;
			lockType = GL_FLOAT;
			break;

		case PF_FLOAT_RGB:
			internalFormat = GL_RGB32F;
			lockFormat = GL_RGB;
			lockType = GL_FLOAT;
			break;

		case PF_FLOAT_RGBA:
			internalFormat = GL_RGBA32F;
			lockFormat = GL_RGBA;
			lockType = GL_FLOAT;
			break;

		case PF_DEPTH_16: 
			internalFormat = GL_DEPTH_COMPONENT16_ARB;
			lockFormat = GL_DEPTH_COMPONENT;
			lockType = GL_FLOAT;
			break;

		case PF_DEPTH_24: 
			internalFormat = GL_DEPTH_COMPONENT24_ARB;
			lockFormat = GL_DEPTH_COMPONENT;
			lockType = GL_FLOAT;
			break;

		case PF_DEPTH_32: 
			internalFormat = GL_DEPTH_COMPONENT32_ARB;
			lockFormat = GL_DEPTH_COMPONENT;
			lockType = GL_FLOAT;
			break;

		default:
			assert(false);
		}
	}

	size_t GLTexture::_bytesPerPixel(PixelFormat format)
	{
		switch (format)
		{
		case PF_UBYTE_R: return sizeof(unsigned char)* 1; break;
		case PF_UBYTE_RG: return sizeof(unsigned char)* 2; break;
		case PF_UBYTE_RGB: return sizeof(unsigned char)* 3; break;
		case PF_UBYTE_RGBA: return sizeof(unsigned char)* 4; break;
		case PF_FLOAT_R: return sizeof(float)* 1; break;
		case PF_FLOAT_RG: return sizeof(float)* 2; break;
		case PF_FLOAT_RGB: return sizeof(float)* 3; break;
		case PF_FLOAT_RGBA: return sizeof(float)* 4; break;
		case PF_DEPTH_16: return sizeof(unsigned char)* 2; break;
		case PF_DEPTH_24: return sizeof(unsigned char)* 3; break;
		case PF_DEPTH_32: return sizeof(unsigned char)* 4; break;
		}
		return 0;
	}

}