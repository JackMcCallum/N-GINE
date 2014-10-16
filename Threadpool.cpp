#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <Windows.h>
#include <assert.h>
#include <condition_variable>

class Conditional
{
public:
	Conditional() :
		mSignalled(false)
	{
	}

	inline void notify()
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mSignalled = true;
		mConditional.notify_one();
	}

	inline void wait()
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mConditional.wait(lock, [&](){ return mSignalled; });
		mSignalled = false;
	}

private:
	std::mutex mMutex;
	std::condition_variable mConditional;
	bool mSignalled;
};

namespace NGine
{
	class ThreadPool;

	class Threadable
	{
		friend class ThreadPool;

	public:
		Threadable() : mIsInQueue(false) {}
		virtual ~Threadable() {}
		virtual void execute() = 0;

		void wait()
		{
			// This task was not put into a queue
			assert(!mIsInQueue);
			mWaiter.wait();
		}

	private:
		Conditional mWaiter;
		bool mIsInQueue;
	};

	class ThreadPool
	{
	public:
		// Entering less than 0 will will mean recommended - amount so:
		// entering -1 on a quad core would make 3, but -1 on an 8 core would make 7
		// Entering 0, will create the recommended amount
		// Entering anything above 0 will make that many threads
		ThreadPool(int numThreads = 0)
		{
			if (numThreads < 0)
				mNumThreads = std::thread::hardware_concurrency() + numThreads;
			if (numThreads == 0)
				mNumThreads = std::thread::hardware_concurrency();
			else
				mNumThreads = numThreads;

			if (mNumThreads < 1)
				numThreads = 1;

			mExit = false;
			mWorkers = new std::thread[mNumThreads];
			mWaiters = new Conditional[mNumThreads];

			// Start all the threads
			for (int i = 0; i < mNumThreads; i++)
				mWorkers[i] = std::thread(&ThreadPool::_threadMain, this, i);
		}
		~ThreadPool()
		{
			mExit = true;
			for (int i = 0; i < mNumThreads; i++)
			{
				mWaiters[i].notify();
				mWorkers[i].join();
			}
			delete[] mWorkers;
			delete[] mWaiters;
		}

		void enqueue(Threadable* task)
		{
			// Cant enqueue a Threadable twice
			assert(task->mIsInQueue);
			mLock.lock();
			mTasks.push_back(task);
			task->mIsInQueue = true;

			// Notify the threads theres work to do
			for (int i = 0; i < mNumThreads; i++)
				mWaiters[i].notify();
			mLock.unlock();
		}

	private:
		void _threadMain(int id)
		{
			while (!mExit)
			{
				mLock.lock();
				if (mTasks.size() == 0)
				{
					mLock.unlock();
					mWaiters[id].wait();
				}
				else
				{
					Threadable* task = mTasks.back();
					mTasks.pop_back();
					mLock.unlock();

					task->execute();
					task->mIsInQueue = false;
					task->mWaiter.notify();
				}
			}
		}

	private:
		std::thread* mWorkers;
		Conditional* mWaiters;
		int mNumThreads;
		bool mExit;

		std::mutex mLock;
		std::vector<Threadable*> mTasks;
	};
}


class TestTask : public NGine::Threadable
{
public:

	virtual void execute()
	{
		int a = 0;
		for (int i = 0; i < 2000000; i++)
		{
			a += 10;
			a %= 3;
			a += 10;
			a /= 2;
		}
	}

};

class Foo
{
public:
	virtual ~Foo()
	{
		std::cout << "Destroying Foo" << std::endl;
	}
};

class Bar : public Foo
{
public:
	virtual ~Bar()
	{
		std::cout << "Destroying Bar" << std::endl;
	}
};


int main()
{
	/*const int numTasks = 80;

	TestTask* tasks = new TestTask[numTasks];

	int c = 0;
	std::cout << "Single threaded" << std::endl;
	c = clock();
	for (int i = 0; i < numTasks; i++)
		tasks[i].execute();
	std::cout << clock() - c << std::endl;


	NGine::ThreadPool threadPool(std::thread::hardware_concurrency());
	std::cout << "Multi threaded (" << std::thread::hardware_concurrency() << " threads)" << std::endl;

	c = clock();
	threadPool.enqueue(&tasks[0]);

	// Enqueue all the tasks
	for (int i = 0; i < numTasks; i++)
		threadPool.enqueue(&tasks[i]);

	// Wait for all tasks to complete
	for (int i = 0; i < numTasks; i++)
		tasks[i].wait();

	std::cout << clock() - c << std::endl;



	delete[] tasks;
	*/

	{ Bar b; }

	system("pause");
	return 0;
}


