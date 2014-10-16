#include "NGineStable.h"
#include "NGineMain.h"
#include "NGineRenderSystem.h"
#include "NGineRenderWindow.h"
#include "NGineMesh.h"
#include "NGineShader.h"
#include "NGineResourceManager.h"

namespace NGine
{
	Main* Main::mSingleton = nullptr;
	RenderSystem* Main::mRenderSystem = nullptr;
	ResourceManager* Main::mResourceManager = nullptr;

	Main::Main(RenderWindow* renderWindow) :
		mInitialized(false),
		mRenderWindow(renderWindow)
	{
		// Do not create more than one NGine::Main
		if (mSingleton)
		{
			Log::error("ERROR: Do not create more than one instance of NGine::Main, it is a singleton class");
			exit(-1);
		}

		mSingleton = this;
	}

	Main::~Main()
	{
		shutdown();
	}

	bool Main::initialize()
	{
		if (mInitialized)
		{
			Log::error("ERROR: NGine::Main is already initialized");
			return false;
		}

		// Validate the config data
		if (!mRenderWindow)
		{
			Log::error("ERROR: NGine::Main was not provided with an NGine::RenderWindow");
			return false;
		}

		mRenderSystem = new RenderSystem(mRenderWindow);
		assert(mRenderSystem);

		// Start the render thread!!!!!
		mRenderSystem->startThread();

		mResourceManager = new ResourceManager();

		mInitialized = true;
		return true;
	}

	void Main::shutdown()
	{
		// Shutdown the resource manager so it has a chance to clean up GPU resources,
		// For this it depends on the render thread still being up and running
		_DELETE_IF(mResourceManager);
		
		// Now shutdown the render system and render thread
		if (mRenderSystem)
			mRenderSystem->exitThread();
		_DELETE_IF(mRenderSystem);

		// Finally shutdown the render window and we are done here
		_DELETE_IF(mRenderWindow);

		mInitialized = false;
	}

	Main& Main::getSinglton()
	{
		assert(mSingleton);
		return *mSingleton;
	}

	RenderSystem& Main::getRenderSystem()
	{
		assert(mRenderSystem);
		return *mRenderSystem;
	}

	ResourceManager& Main::getResourceManager()
	{
		assert(mResourceManager);
		return *mResourceManager;
	}

}