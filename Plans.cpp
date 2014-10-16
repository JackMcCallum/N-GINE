// Plans for how i want the base application to look when finished


// Abstract class providing a thread
NGine::RenderThread

// Provides a window/context to render to using SDL
NGine::SDLRenderThread

// Provites a window/context to render to using Qt
NGine::QtRenderThread

// Provides a main class from which you can retreive pointers to all other singleton classes such as:
//  - ResourceFactory
//  - SceneFactory
//  - RenderThread
NGine::Main
NGine::Main::renderOneFrame();
NGine::Main::getResourceFactory();
NGine::Main::getSceneFactory();
NGine::Main::getRenderThread();

// A factory from which you can spawn resources such as materials/meshes/textures/rendertargets
NGine::ResourceFactory

// A factory class from which you can spawn things that will belong in the scene such as cameras/nodes/entities/lights
NGine::SceneFactory

// Some things NGine::SceneFactory will provide
NGine::Camera
NGine::SceneNode
NGine::Entity

// Some things NGine::ResourceFactory will provide
NGine::Resource
NGine::Texture
NGine::Mesh
NGine::Material
NGine::Shader
NGine::RenderTarget


// Pseudo code for how a base application should look

class BaseApplication
{
public:
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy() = 0;
	
	int run()
	{
		mRenderThread = new NGine::SDLRenderThread(..., ...);
		... OR ...
		mRenderThread = new NGine::QtRenderThread(..., ...);
		
		// Setup the config for the engine
		NGine::Main::Config config;
		config.variousSettings = ...;
		config.renderThread = mRenderThread;
		config.variousSettings = ...;
		
		// Create main
		mNGineMain = new NGine::Main(config);
		
		// Initialize N-GINE
		if (!mNGineMain->initialize())
		{
			NGine::Log::showMessage("ERROR: Could not initialize N-GINE, see log for details.");
			return -1;
		}
		
		// Begin the game loop
		onCreate();
		while (~mNGineMain->shouldClose())
		{
			// Calculate delta time here probably
			onUpdate(deltaTime);
			mNGineMain->renderOneFrame();
		}
		onDestroy();
		
		// Clean up before exit
		delete mApp;
		delete mMain;
		delete mRenderThread;
		
		// Success
		return 0;
	}
}

// Pseudo code for how the main application should get entry to NGine

class Application : public BaseApplication
{
public:
	virtual void onCreate()
	{
		// Set up all your scene propaganda
		SceneFactory* sf = NGine::Main::getSceneFactory();
		Scene* mDisplayScene = sf->createScene("DisplayScene");
		SceneNode* root = mDisplayScene->getRootNode();
		
		// This does not belong to N-GINE
		mComponent = new CustomComponent();
		
		SceneNode* someNode = root->createChild("SomeNode");
		someNode->attachComponent(mComponent);
	}
	
	virtual void onUpdate()
	{
		// Make changes you want to see in the next frame
	}
	
	virtual void onDestroy()
	{
		// Clean up anything that doesn't belong to NGine
		delete mComponent;
		
		// N-GINE is cleaned up in the base class after returning from here
	}
	// ...
	
	CustomComponent* mComponent;
	Scene* mDisplayScene;
}



int main()
{
	Application* app = new Application();
	int status = app->run();
	delete app;
	return status;
}


What is N-GINE?
N-GINE is a threaded render system
it comes with a scene graph
to which components can be attached
it needs to be extensible
Resource Managment



What is N-GINE not?
Not a game engine
not a window managment system














