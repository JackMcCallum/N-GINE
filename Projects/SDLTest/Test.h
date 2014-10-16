
#include "NGineRenderThread.h"
#include "NGineRenderCommand.h"

#include "sdl/SDL.h"
#include <iostream>

#include <glew/glew.h>
#include <gl/GL.h>
/*
class ClearCommand : public NGine::RenderCommand
{
public:
	virtual void execute()
	{
		glClearColor(rand() / float(RAND_MAX), 32 / 255.0f, 32 / 255.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

class Foo : public NGine::RenderThread
{
public:
	Foo() : NGine::RenderThread() {}

	void onCreate()
	{
		RenderThread::initialize(1024, 1024);
	}

	void onUpdate()
	{

	}

	void onDestroy()
	{

	}

	virtual void onThreadStart()
	{
		mWindow->gainContext();
	}

	virtual void onThreadEnd()
	{
		mWindow->loseContext();
	}

	virtual void onFrameStart()
	{
	}

	virtual void onFrameEnd()
	{
		mWindow->swapBuffers();
	}

	NGine::RenderWindow* mWindow;
};


void testMain()
{
	SDL_Init(SDL_INIT_VIDEO);

	NGine::RenderWindow* window = new NGine::RenderWindow();
	window->initialize(1280, 720, "Test Window", 4, true);
	window->loseContext();

	Foo* app = new Foo();
	app->mWindow = window;
	app->startThread();
	
	// Game loop
	app->onCreate();
	while (true)
	{
		// Check if window was closed
		if (window->wasClosed())
		{
			app->exitThread();
			break;
		}

		// Poll events
		window->messagePump();
		app->onUpdate();


		ClearCommand testcmd;
		app->enqueueCommand(testcmd);
		app->swapQueue();
	}
	app->onDestroy();

	delete window;
	delete app;
	SDL_Quit();
	}*/