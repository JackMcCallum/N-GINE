#ifndef __NGINE_RENDER_WINDOW_H_
#define __NGINE_RENDER_WINDOW_H_

#include "NGineRenderTarget.h"

namespace NGine
{
	class RenderWindow : public RenderTarget
	{
	public:
		virtual ~RenderWindow() {}

		virtual void onMakeCurrent() = 0;
		virtual void onDoneCurrent() = 0;
		virtual void onSwapBuffers() = 0;

		virtual void setActive();
	};
}

#endif // !__NGINE_RENDER_WINDOW_H_
