#ifndef __NGINE_CONDITIONAL_H_
#define __NGINE_CONDITIONAL_H_

#include "NGineCommon.h"
#include "NGineLog.h"
#include <mutex>
#include <condition_variable>

namespace NGine
{
	class Conditional
	{
	public:
		Conditional() :
			mSignalled(false)
		{
			//mPerfTmp = SDL_GetPerformanceCounter();
			//mPercWaiting = 0;
		}

		inline void notify()
		{

			std::unique_lock<std::mutex> lock(mMutex);
			mSignalled = true;
			mConditional.notify_all();


		}

		inline void wait()
		{
			//mPerfNotWaiting = SDL_GetPerformanceCounter() - mPerfTmp;
			//mPerfTmp = SDL_GetPerformanceCounter();

			std::unique_lock<std::mutex> lock(mMutex);
			mConditional.wait(lock, [&](){ return mSignalled; });
			mSignalled = false;

			//mPerfWaiting = SDL_GetPerformanceCounter() - mPerfTmp;
			//mPerfTmp = SDL_GetPerformanceCounter();

			//mPercWaiting = mPercWaiting*0.99 + (double)mPerfWaiting / ((double)mPerfNotWaiting + (double)mPerfWaiting) * 0.01;
		}

		//inline float percentageWaitingTime()
		//{
		//	return (float)mPercWaiting;
		//}


	private:
		std::mutex mMutex;
		std::condition_variable mConditional;
		bool mSignalled;

		//Uint64 mPerfNotWaiting;
		//Uint64 mPerfWaiting;
		//Uint64 mPerfTmp;
		//double mPercWaiting;
	};
}

#endif // !__NGINE_CONDITIONAL_H_
