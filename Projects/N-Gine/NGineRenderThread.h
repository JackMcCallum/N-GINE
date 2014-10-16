#ifndef __RENDER_SYSTEM_H_
#define __RENDER_SYSTEM_H_

#include <atomic>
#include "NGineCommon.h"
#include "NGineConditional.h"
#include "NGineCommandQueue.h"

namespace NGine
{

	/*
	@author
	Jack McCallum

	@description
	Render thread, an abstract class that starts up a thread from which you
	can enqueue commands then swap the buffers and it will execute the commands in 
	the order you enqueued them. when finished it will idle and wait for the next swap

	It does not handle any rendering things itself but for you to render with OpenGL it requires
	an active context on the thread, which you can access through onThreadStart() onThreadEnd()
	onFrameStart() and onFrameEnd().

	*/

	class RenderCommand;
	class RenderWindow;

	class RenderThread
	{
	public:
		RenderThread(RenderWindow* window);
		virtual ~RenderThread();

		// Called when thread has been created, before anything
		virtual void onThreadStart() = 0;

		// Last thing called before the thread is exited
		virtual void onThreadEnd() = 0;

		// Do not be fooled by the names of these functions
		// You are required to supply a thread with an active GL context
		// you then need to call startThread(); which will begin the infinite loop
		// start thread will not return until exitThread(); is called
		void startThread();

		// Tells the thread to exit and waits until it does. does not return until thread has ended.
		void exitThread();

		// Returns true if called on the render thread
		bool isRenderThread();

		// Returns true if the render thread is up and running
		bool isThreadRunning();

		// Block the calling thread until all commands currently in the queue are completed
		void flushQueue();

		// Enqueue a command to swap the buffers of the window
		void swapBuffers();

		// copies a command into the queue. this uses a frame allocator and after the command is
		// executed the destructor is called. when all commands finished, allocator is cleared.
		// A command should not contain any lose pieces of memory, this would risk the performance of the rendering.
		// A command is primarily designed to be used with raw data and render commands.
		// If you need arbitrary ram see allocateScratch();
		template<class T>
		inline void enqueueCommand(const T& cmd)
		{
			assert(isThreadRunning());
			mCommandQueue.enqueueCommand(cmd);
		}

		void* allocateScratch(size_t size) { return nullptr; }

	protected:
		void _threadMain();

	protected:
		CommandQueue mCommandQueue;

		// Some state vars
		std::atomic<bool> mThreadRunning;

		// Conditional vars used to synchronizing the thread
		NGine::Conditional mCondThreadReady;

		// The render threads handle
		std::thread mThread;
		std::thread::id mThreadId;

		RenderWindow* mRenderWindow;

	};

}

#endif // !__RENDER_SYSTEM_H_


