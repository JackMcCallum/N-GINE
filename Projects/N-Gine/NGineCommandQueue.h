#ifndef __NGINE_COMMAND_QUEUE_H_
#define __NGINE_COMMAND_QUEUE_H_

#include <mutex>
#include <queue>
#include <condition_variable>
#include "NGineCommon.h"
#include "NGineFixedSizeAllocator.h"

namespace NGine
{
	// Simple abstract command class
	class Command
	{
	public:
		virtual ~Command() {}

		virtual void executeAndDestroy() = 0;
	};

	// A command queue from which you can push commands and have them execute on
	// a second thread primarily built for render commands on a 2nd thread
	// To make a command for the queue have a class that inherits from TCommand
	// with the template paramter as the class name you just made
	// And override the abstract execute() method and put your code in there
	// eg: class MyCommand : public TCommand<MyCommand>
	class CommandQueue
	{
	public:
		CommandQueue() : mRequestReturn(false) {}

		// Enter an infinite loop processing each task as it can
		// This method wont return until requestReturn() is called
		void processUntilRequestReturn();

		// Request the return of the infinite loop
		void requestReturn();

		// Take a task from the queue
		// Ownership is lost
		Command* popCommand();

		// Push a task into the queue
		template<class T>
		void enqueueCommand(const T& task);

	protected:
		template<class T>
		friend class TCommand;

		enum TaskSize
		{
			SMALL_TASK = 64,
			MEDIUM_TASK = 256,
			LARGE_TASK = 1024,
		};

		// Task allocators
		static TFixedSizeAllocator<SMALL_TASK>& getSmallTaskAllocator();
		static TFixedSizeAllocator<MEDIUM_TASK>& getMediumTaskAllocator();
		static TFixedSizeAllocator<LARGE_TASK>& getLargeTaskAllocator();

	private:
		std::queue<Command*> mTaskQueue;
		std::mutex mQueueMutex;

		bool mRequestReturn;
	};

	template<class T>
	void CommandQueue::enqueueCommand(const T& task)
	{
		mQueueMutex.lock();

		if (sizeof(T) <= SMALL_TASK)
		{
			void* ptr = CommandQueue::getSmallTaskAllocator().allocate();
			mTaskQueue.push(new (ptr)T(task));
		}
		else if (sizeof(T) <= MEDIUM_TASK)
		{
			void* ptr = CommandQueue::getMediumTaskAllocator().allocate();
			mTaskQueue.push(new (ptr)T(task));
		}
		else if (sizeof(T) <= LARGE_TASK)
		{
			void* ptr = CommandQueue::getLargeTaskAllocator().allocate();
			mTaskQueue.push(new (ptr)T(task));
		}
		else
		{
			mTaskQueue.push(new T(task));
		}

		mQueueMutex.unlock();
	}

	template<class T>
	class TCommand : public Command
	{
	public:
		virtual ~TCommand() {}

		// Virtual function for the user to put the code
		// they want to execute in
		virtual void execute() = 0;

		// Execute the command and clean it up after completion
		virtual void executeAndDestroy()
		{
			// Execute the task
			execute();

			// Destroy the task
			if (sizeof(T) <= CommandQueue::SMALL_TASK)
			{
				this->~TCommand();
				CommandQueue::getSmallTaskAllocator().free(this);
			}
			else if (sizeof(T) <= CommandQueue::MEDIUM_TASK)
			{
				this->~TCommand();
				CommandQueue::getMediumTaskAllocator().free(this);
			}
			else if (sizeof(T) <= CommandQueue::LARGE_TASK)
			{
				this->~TCommand();
				CommandQueue::getLargeTaskAllocator().free(this);
			}
			else
			{
				delete this;
			}
		}
	};

	// Command queue fence can be used to track the command queue,
	// It can be used to flush the queue or wait for a command to complete
	class CommandQueueFence
	{
	public:
		class NotifyEvent : public NGine::TCommand<NotifyEvent>
		{
		public:
			NotifyEvent(CommandQueueFence* parent) : mParent(parent) {}

			virtual void execute() { mParent->notify(); }

		private:
			CommandQueueFence* mParent;
		};

	public:
		CommandQueueFence() : mSignalled(false), mHasBegun(false), mNotifyEvent(this) {}

		// Begin the fence, inserts a command into the given queue
		// When that command is processed it will call notify();
		void begin(NGine::CommandQueue* queue);

		// notify the fence it has completed
		void notify();

		// Wait for the fence to complete, if it has already completed
		// it will do nothing
		void wait();

	private:
		std::mutex mMutex;
		std::condition_variable mConditional;
		bool mSignalled;
		bool mHasBegun;

		NotifyEvent mNotifyEvent;
	};
}

#endif // !__NGINE_COMMAND_QUEUE_H_
