#ifndef __NGINE_RENDER_TARGET_H_
#define __NGINE_RENDER_TARGET_H_

#include "NGineCommon.h"
#include "NGineComponent.h"
#include <glm/glm.hpp>

namespace NGine
{

	class RenderTarget
	{
		typedef std::vector<Renderable::DrawCall>::iterator DrawCallIterator;

	public:
		RenderTarget();
		virtual ~RenderTarget();

		// Updates updates this viewport with new data, this relies on the render system outliving the viewport
		virtual void updateTarget();

		// Sets whether the core automatically updates this target when you call core->renderFrame(); 
		// otherwise, you will have to update it yourself
		void setAutoUpdate(bool update);
		// Get whether this target is auto updated
		bool isAutoUpdated() const;

		// Set the size of the render texture.
		// Note: this DOES NOT resize the attached textures, the size values are
		//		 only for telling OpenGL what size the viewport needs to be
		virtual void setDimensions(uint32 width, uint32 height);

		void setCamera(Camera* camera);

		void renderQuad(MaterialPtr material);

	protected:
		// Set this as the active render target for the render system
		virtual void setActive() = 0;

		bool _readyDrawCalls();
		void _iterateDrawcalls(DrawCallIterator start, DrawCallIterator end);


	protected:
		uint32 mWidth;
		uint32 mHeight;
		Camera* mCamera;

		// A cached renderables list, so we don't have to keep creating a new list for each frame
		// Keeps the memory allocated permanently
		std::vector<Renderable*> mCachedRenderableList;
		std::vector<Renderable::DrawCall> mCachedDrawCallList;
		DrawCallIterator mDrawCallIterator;
		glm::mat4 mWorldMatrixCache[512];
		uint32 mTriangleCount;
		uint32 mBatchCount;
		uint32 mSavedByBatching;
	};
}

#endif // !__NGINE_RENDER_TARGET_H_
