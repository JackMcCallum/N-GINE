#ifndef __NGINE_FIXED_SIZE_ALLOCATOR_H_
#define __NGINE_FIXED_SIZE_ALLOCATOR_H_

#include "NGineCommon.h"

namespace NGine
{
	/*
	this allocator will return a block a memory of size 'SIZE' in bytes
	when this allocator runs out of free memory it will allocate more
	it will allocate 'RESERVE' many and return 1 and save the rest for later
	RESERVE = 64 by default

	this allocators primary use is for the command queue
	to speed up the storage of thousands of commands 
	*/

	
	template<uint32 SIZE, uint32 RESERVE = 64>
	class TFixedSizeAllocator
	{
	public:
		~TFixedSizeAllocator()
		{
			for (auto iter : mBlockList)
				::free(iter);

			assert(mFreeList.size() == mNumAllocated, "Possible memory leaks");
		}

		void* allocate()
		{
			static_assert(RESERVE > 0, "Reserve cannot be 0");

			mLock.lock();

			if (!mFreeList.empty())
			{
				// Grab a free pointer from the back of the list
				void* ptr = mFreeList.back();
				mFreeList.pop_back();

				assert(ptr);

				mLock.unlock();
				return ptr;
			}
			else
			{
				// Allocate a new block
				void* block = malloc(SIZE * RESERVE);
				if (!block) throw std::exception("Out of memory");

				mBlockList.push_back(block);

				for (uint32 i = 1; i < RESERVE; i++)
					mFreeList.push_back((uint8*)block + SIZE * i);

				mNumAllocated += RESERVE;

				mLock.unlock();
				return block;
			}
		}

		void free(void* ptr)
		{
			assert(ptr);
			mLock.lock();
			mFreeList.push_back(ptr);
			mLock.unlock();
		}

	private:
		// List of free slots
		std::vector<void*> mFreeList;
		std::vector<void*> mBlockList;
		uint32 mNumAllocated;
		std::mutex mLock;
	};
	
	template<class Type, uint32 RESERVE = 64>
	class TFixedTypeAllocator
	{
	public:
		Type* allocate()
		{
			static_assert(RESERVE > 0, "Reserve cannot be 0");
			return (Type*)mAlloc.allocate();
		}

		void free(Type* ptr)
		{
			mAlloc.free(ptr);
		}

	private:
		TFixedSizeAllocator<sizeof(Type), RESERVE> mAlloc;
	};

	template<class T>
	class TPoolAllocator
	{
	public:
		void* operator new(size_t size)
		{
			assert(size == sizeof(T));
			return getAllocator().allocate();
		}

		void operator delete(void* ptr)
		{
			getAllocator().free((T*)ptr);
		}

		static TFixedTypeAllocator<T>& getAllocator()
		{
			static TFixedTypeAllocator<T> alloc;
			return alloc;
		}
	};
}

#endif // !__NGINE_FIXED_SIZE_ALLOCATOR_H_
