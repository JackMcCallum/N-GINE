#ifndef __NGINE_BUFFER_H_
#define __NGINE_BUFFER_H_

#include "NGineCommon.h"
#include "NGineRenderThread.h"

namespace NGine
{
	class RenderSystem;

	class GLMesh;
	class GLBuffer
	{
		friend class RenderSystem;
		friend class GLMesh;

	public:
		friend class GLMesh;
		GLBuffer(size_t size = 0, BufferUsage usage = BU_STATIC);
		~GLBuffer();
		
		void* lock(size_t offset = 0, size_t length = 0);
		void unlock();

		void resize(size_t size, BufferUsage usage);

		size_t getSize();

	private:
		static GLenum _translateUseage(BufferUsage usage);
		
		void _init();
		void _release();

	private:
		bool mInitialized;

		// Regular data
		size_t mSize;
		BufferUsage mUsage;

		// Lock data
		void* mLockData;
		bool mLockDeleteAfter;
		size_t mLockOffset;
		size_t mLockLength;
		
		// Data owned by the render thread, this is garbage collected
		// so that it will outlive the class if it needs to
		struct RenderData
		{
			uint32 bufferHandle;
		};
		// Garbage collected render data
		typedef std::shared_ptr<RenderData> RenderDataPtr;
		RenderDataPtr mRenderData;
	};
}

#endif // !__NGINE_BUFFER_H_
