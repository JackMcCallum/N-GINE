#pragma once
#ifndef __NGINE_GL_FRAME_BUFFER_H_
#define __NGINE_GL_FRAME_BUFFER_H_

namespace NGine
{
	class RenderSystem;
	class GLTexture2D;

	class GLFrameBuffer final : public TPoolAllocator<GLFrameBuffer>
	{
		friend class RenderSystem;

	public:
		GLFrameBuffer();
		~GLFrameBuffer();

		// Attach a 2D texture to the frame buffer
		void attachGLTexture2D(GLTexture2D* texture, GLenum attachment);

		// Returns the status of the frame buffer
		// Be warned this function forces the render queue to flush
		// if ret != GL_FRAMEBUFFER_COMPLETE, buffer is invalid
		GLenum getStatus();
		


	private:
		void _init();
		void _release();

	private:
		bool mInitialized;

		// Data owned by the render thread, this is garbage collected
		// so that it will outlive the class if it needs to
		struct RenderData
		{
			uint32 frameBufferHandle;
		};
		// Garbage collected render data
		typedef std::shared_ptr<RenderData> RenderDataPtr;
		RenderDataPtr mRenderData;
	};
}

#endif // !__NGINE_GL_FRAME_BUFFER_H_
