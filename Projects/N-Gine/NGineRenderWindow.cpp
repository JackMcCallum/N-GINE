#include "NGineStable.h"
#include "NGineRenderWindow.h"
#include "NGineMain.h"
#include "NGineRenderSystem.h"

namespace NGine
{
	void RenderWindow::setActive()
	{
		RenderSystem& rs = Main::getRenderSystem();
		rs.bindFrameBuffer(nullptr);
		rs.setViewportMode(glm::vec4(0, 0, mWidth, mHeight));
	}
}

