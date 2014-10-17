#ifndef __NGINE_TEXTURE_H_
#define __NGINE_TEXTURE_H_

#include "NGineCommon.h"
#include "NGineResource.h"
#include <FreeImage/FreeImage.h>

namespace NGine
{
	class GLTexture2D;
	class GLTexture;
	class ImageFile;

	RESOURCE_PTR(Texture);
	class Texture final : public Resource, public TPoolAllocator<Texture>
	{
		friend class ResourceManager;
	private:
		Texture(const std::string& name, const std::string fname);
		// Ending of each image file for cube maps in the order of +X -X +Y -Y +Z -Z
		// for example "./CubeMap_Left.png" you'd put in fname "./CubeMap_.png" and in 'ending'
		// "Left Right Top Down Front Back" separated with spaces
		Texture(const std::string& name, const std::string fname, const std::string& ending);
		// Create a manual texture that can be used 
		Texture(const std::string& name, uint32 width, uint32 height, PixelFormat format, uint32 mipLevels = 1);
		//Texture(const std::string& name, const std::string& fname);
		~Texture();

	public:
		bool prepare();
		bool unprepare();
		bool load();
		bool unload();
		
		GLTexture* getGLTexture();
		
	private:
		// The storage format of the texture
		TextureType mTextureType;
		
		// The handle to the raw texture
		GLTexture* mGLTexture;

		bool mManual;
		std::string mFileName;
		std::string mEndings;

		// One for each side of the cube map if needed
		// Or index 0 for a 2D texture
		ImageFile* mImageFile[6];
	};
}

#endif // !__NGINE_TEXTURE_H_
