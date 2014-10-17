#ifndef __NGINE_DEFERRED_RENDER_TARGET_H_
#define __NGINE_DEFERRED_RENDER_TARGET_H_

#include "NGineCommon.h"
#include "NGineRenderTarget.h"
#include "NGineResource.h"
#include "NGineGLFrameBuffer.h"

namespace NGine
{
	RESOURCE_PTR(Texture);

	class DeferredRenderTarget : public RenderTarget
	{
	public:
		DeferredRenderTarget(uint32 width = 1280, uint32 height = 720);
		virtual ~DeferredRenderTarget();

		virtual void updateTarget();
		virtual void setActive();

	private:
		void _lightPass();
		void _postProcess();

		// GBuffer
		GLFrameBuffer* mGBuffer;
		TexturePtr mAlbedoAO; // 8MB @1080p
		TexturePtr mNormal; // 24MB @1080p
		TexturePtr mEmissive; // 6MB @1080p
		TexturePtr mSurface; // 4MB @1080p

		// HDR Final buffer
		GLFrameBuffer* mHDRBuffer; 
		TexturePtr mHDRTexture; // 24MB @1080p

		// Shared
		TexturePtr mDepth24; // 8MB @1080p

		MaterialPtr mMatAmbientPass;
		MaterialPtr mMatDirLight;
		MaterialPtr mMatPointLight;

		std::vector<Light*> mCachedLightList;

	};
}

#endif // !__NGINE_DEFERRED_RENDER_TARGET_H_
