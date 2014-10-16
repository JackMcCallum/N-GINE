#ifndef __NGINE_CORE_H_
#define __NGINE_CORE_H_

#include "NGineCommon.h"

namespace NGine
{
#define ngMain NGine::Main::getSinglton()
#define ngRenderSys NGine::Main::getRenderSystem()
#define ngResourceMgr NGine::Main::getResourceManager()

	class RenderSystem;
	class ResourceManager;

	class Main
	{
	public:
		Main(RenderWindow* renderWindow);
		~Main();

		bool initialize();
		void shutdown();

		static Main& getSinglton();
		static RenderSystem& getRenderSystem();
		static ResourceManager& getResourceManager();

	private:
		bool mInitialized;

		static Main* mSingleton;
		static RenderSystem* mRenderSystem;
		static ResourceManager* mResourceManager;
		RenderWindow* mRenderWindow;
	};
}

#endif // !__NGINE_CORE_H_
