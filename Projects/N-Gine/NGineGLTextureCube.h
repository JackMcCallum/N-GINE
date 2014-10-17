#ifndef __NGINE_HARDWARE_CUBEMAP_H_
#define __NGINE_HARDWARE_CUBEMAP_H_

#include "NGineCommon.h"
#include "NGineGLTexture.h"

namespace NGine
{
	class GLTextureCube : public GLTexture, public TPoolAllocator<GLTextureCube>
	{
		friend class RenderSystem;
		friend class GLFrameBuffer;

	public:
		GLTextureCube(uint16 width, uint16 height, PixelFormat format, uint8 mips = 1);
		~GLTextureCube();

		virtual GLTextureCube* getTextureCube() override { return this; }

		uint16 getWidth(uint8 level = 0) const;
		uint16 getHeight(uint8 level = 0) const;
		uint8 getNumMips() const;
		PixelFormat getPixelFormat() const;

		void* lock(uint32 face, uint16 x = 0, uint16 y = 0, uint16 w = 0, uint16 h = 0, uint8 level = 0, bool genMips = false);
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
			uint8 face;
			void* data;
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

#endif // !__NGINE_HARDWARE_CUBEMAP_H_
