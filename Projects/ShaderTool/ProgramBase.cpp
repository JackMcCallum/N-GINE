#include "Precompiled.h"
#include "ProgramBase.h"

ProgramBase::ProgramBase() :
mWindow(nullptr),
mShouldClose(false)
{
}

ProgramBase::~ProgramBase()
{
}

bool ProgramBase::run()
{
	mWindow = new NGine::RenderWindow();
	if (!mWindow || !mWindow->initialize(1280, 720, "N-Gine Shader Tool", 0, false))
		return false;
	
	mWindow->addListener(this);
	eventCreate();
	
	double stime = mWindow->getTime();
	while (!mShouldClose && !mWindow->shouldClose())
	{
		// Calculate delta time
		mTotalTime = mWindow->getTime();
		mDeltaTime = mTotalTime - stime;
		stime = mTotalTime;
	
		eventUpdate();
		eventDraw();
		mWindow->onSwapBuffers();
		mWindow->messagePump();
	}
	
	eventDestroy();
	mWindow->removeListener(this);
	
	delete mWindow;
	mWindow = nullptr;
	
	return true;
}

void ProgramBase::close()
{
	mShouldClose = true;
}

NGine::RenderWindow* ProgramBase::getWindow() const
{
	return mWindow;
}

double ProgramBase::deltaTime() const
{
	return mDeltaTime;
}

double ProgramBase::totalTime() const
{
	return mTotalTime;
}
