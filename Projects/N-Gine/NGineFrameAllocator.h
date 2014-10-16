#ifndef __NGINE_FRAME_ALLOCATOR_H_
#define __NGINE_FRAME_ALLOCATOR_H_

#include "NGineCommon.h"
#include "NGineLog.h"

namespace NGine
{
	/*
		@author
		Jack McCallum

		@description
		Very simple frame allocator
		memory to be used once per frame, then cleared all at once
		for extremely fast arbitrary memory allocation
	*/

	class FrameAllocator
	{
	public:
		// if size is not 0 it will initialize for you
		// reallocateOnClear will reallocate on clear if the capacity was reached last frame
		FrameAllocator(size_t size = 0) :
			mStart(nullptr),
			mEnd(nullptr),
			mNext(nullptr),
			mHasWarnedOOM(false)
		{
			if (size > 0)
				initialize(size);
		}

		~FrameAllocator()
		{
			if (mStart)
				free(mStart);
		}

		// Initialize the memory allocator with n number of bytes
		inline bool initialize(size_t size)
		{
			mSize = size;
			if (mStart)
			{
				free(mStart);
				mStart = nullptr;
			}
			mStart = malloc(size);
			if (!mStart)
				return false;
			mEnd = (char*)mStart + size;
			mNext = mStart;
			return true;
		}

		// Clear the allocator of all allocations
		inline void clear()
		{
			mNext = mStart;
		}
		
		// Allocate a chunk of bytes
		inline void* allocate(size_t size)
		{
			void* newNext = ((char*)mNext) + size;
			if (newNext > mEnd)
			{
				if (!mHasWarnedOOM)
				{
					mHasWarnedOOM = true;
					Log::warning("Frame allocator is over capacity.");
				}
				return nullptr;
			}

			void* allocated = mNext;
			mNext = newNext;
			return allocated;
		}

		// Allocate and return a pointer to the newed object
		template<class T>
		inline T* allocate()
		{
			void* mem = allocate(sizeof(T));
			if (!mem) return nullptr;
			return new (mem)T();
		}

		// Allocate a new object with parameters, this is a very dangerous way of doing things
		// as is quite bug prone so its not recommended but made available to convenience
		// Up to 4 arguments
		template<class T, class A>
		inline T* allocate(A a)
		{
			void* mem = allocate(sizeof(T));
			if (!mem) return nullptr;
			return new (mem)T(a);
		}

	private:
		void* mStart;
		void* mEnd;
		void* mNext;
		size_t mSize;
		bool mHasWarnedOOM;
	};
}

#endif // !__NGINE_FRAME_ALLOCATOR_H_
