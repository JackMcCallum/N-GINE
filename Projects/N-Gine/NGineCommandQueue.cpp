#include "NGineStable.h"
#include "NGineCommandQueue.h"

namespace NGine
{

	void CommandQueue::processUntilRequestReturn()
	{
		mRequestReturn = false;
		Command* task = nullptr;
		while (!mRequestReturn)
		{
			task = popCommand();
			if (task)
				task->executeAndDestroy();
		}
	}

	void CommandQueue::requestReturn()
	{
		mRequestReturn = true;
	}

	Command* CommandQueue::popCommand()
	{
		mQueueMutex.lock();

		Command* task = nullptr;
		if (!mTaskQueue.empty())
		{
			task = mTaskQueue.front();
			mTaskQueue.pop();
		}

		mQueueMutex.unlock();
		return task;
	}

	TFixedSizeAllocator<CommandQueue::SMALL_TASK>& CommandQueue::getSmallTaskAllocator()
	{
		static TFixedSizeAllocator<SMALL_TASK> allocator;
		return allocator;
	}

	TFixedSizeAllocator<CommandQueue::MEDIUM_TASK>& CommandQueue::getMediumTaskAllocator()
	{
		static TFixedSizeAllocator<MEDIUM_TASK> allocator;
		return allocator;
	}

	TFixedSizeAllocator<CommandQueue::LARGE_TASK>& CommandQueue::getLargeTaskAllocator()
	{
		static TFixedSizeAllocator<LARGE_TASK> allocator;
		return allocator;
	}

	void CommandQueueFence::begin(NGine::CommandQueue* queue)
	{
		mHasBegun = true;
		mSignalled = false;
		queue->enqueueCommand(mNotifyEvent);
	}

	void CommandQueueFence::notify()
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mSignalled = true;
		mHasBegun = false;
		mConditional.notify_all();
	}

	void CommandQueueFence::wait()
	{
		if (!mSignalled && mHasBegun)
		{
			std::unique_lock<std::mutex> lock(mMutex);
			mConditional.wait(lock, [&](){ return mSignalled; });
		}
	}

}