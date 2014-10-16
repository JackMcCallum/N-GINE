#include "NGineStable.h"
#include "NGineRenderThread.h"
#include "NGineRenderCommand.h"
#include "NGineRenderWindow.h"
#include "NGineFrameAllocator.h"

namespace NGine
{
	RenderThread::RenderThread(RenderWindow* window) :
		mThreadRunning(false),
		mRenderWindow(window)
	{
		assert(mRenderWindow);
	}

	RenderThread::~RenderThread()
	{
		assert(!mThreadRunning, "Thread is still running, call exitThread();");
	}

	void RenderThread::startThread()
	{
		if (!mThreadRunning)
		{
			mRenderWindow->onDoneCurrent();
			mThread = std::thread(&RenderThread::_threadMain, this);
			mCondThreadReady.wait();
		}
		else
		{
			Log::error("ERROR: startThread() was called when the thread is already running");
		}
	}

	void RenderThread::exitThread()
	{
		if (mThreadRunning)
		{
			flushQueue();

			mCommandQueue.requestReturn();

			if (mThread.joinable())
				mThread.join();

			mRenderWindow->onMakeCurrent();

			assert(!isThreadRunning());
		}
	}

	void RenderThread::_threadMain()
	{  
		mThreadId = std::this_thread::get_id();
		mThreadRunning = true;

		// Notify the thread has started
		mRenderWindow->onMakeCurrent();
		onThreadStart();

		// Notify the game thread, that the thread is ready, so it can start putting commands in
		mCondThreadReady.notify();

		mCommandQueue.processUntilRequestReturn();

		// Notify the thread is about to end
		onThreadEnd();
		mRenderWindow->onDoneCurrent();
		mThreadRunning = false;
	}

	bool RenderThread::isRenderThread()
	{
		return (std::this_thread::get_id() == mThreadId);
	}

	bool RenderThread::isThreadRunning()
	{
		return mThreadRunning;
	}

	void RenderThread::flushQueue()
	{
		assert(!isRenderThread());

		CommandQueueFence fence;
		fence.begin(&mCommandQueue);
		fence.wait();
	}

	void RenderThread::swapBuffers()
	{
		assert(!isRenderThread());

		ENQUEUE_RENDER_COMMAND_1PARAMS(Finalize, 
			RenderWindow*, rw, mRenderWindow,
			rw->onSwapBuffers());
	}

	
}

