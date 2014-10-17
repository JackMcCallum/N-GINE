#ifndef __NGINE_GL_TEXTURE2D_H_
#define __NGINE_GL_TEXTURE2D_H_

#include "NGineCommon.h"
#include "NGineGLTexture.h"
#include "NGineMemoryBlock.h"

#define MAX_TEXURE_RESOLUTION 4096

namespace NGine
{
	class RenderSystem;
	class GLFrameBuffer;

	class GLTexture2D : public GLTexture, public TPoolAllocator<GLTexture2D>
	{
		friend class RenderSystem;
		friend class GLFrameBuffer;

	public:
		GLTexture2D(uint16 width, uint16 height, PixelFormat format, uint8 mips = 1);
		~GLTexture2D();

		virtual GLTexture2D* getTexture2D() override { return this; }

		uint16 getWidth(uint8 level = 0) const;
		uint16 getHeight(uint8 level = 0) const;
		uint8 getNumMips() const;
		PixelFormat getPixelFormat() const;

		void* lock(uint16 x = 0, uint16 y = 0, uint16 w = 0, uint16 h = 0, uint8 level = 0, bool genMips = false);
		void unlock();

	private:
		struct TexData
		{
			// Dimensions of the texture
			uint16 width;
			uint16 height;
			uint8 mipLevels;
			PixelFormat pixelFormat;
		} mTexData;
		
		struct LockData
		{
			uint16 x;
			uint16 y;
			uint16 w;
			uint16 h;
			uint8 level;
			MemoryBlock data;
			bool genMips;
		} mLockData;

		bool mIsLocked;

		struct RenderData
		{
			uint32 texHandle;
		};
		typedef std::shared_ptr<RenderData> RenderDataPtr;
		RenderDataPtr mRenderData;
	};

}

#endif // !__NGINE_GL_TEXTURE2D_H_
