#include "NGineStable.h"
#include "NGineMemoryBlock.h"

namespace NGine
{

	MemoryBlock::MemoryBlock() :
		mSize(0),
		mActualSize(0),
		mData(nullptr)
	{
	}

	MemoryBlock::MemoryBlock(size_t size, bool zeroMem) :
		mSize(size),
		mActualSize(0),
		mData(nullptr)
	{
		if (mSize <= SMALL_BLOCK)
		{
			mActualSize = SMALL_BLOCK;
			mData = getSmallAllocator().allocate();
		}
		else if (mSize <= MEDIUM_BLOCK)
		{
			mActualSize = MEDIUM_BLOCK;
			mData = getMediumAllocator().allocate();
		}
		else if (mSize <= LARGE_BLOCK)
		{
			mActualSize = LARGE_BLOCK;
			mData = getLargeAllocator().allocate();
		}
		else
		{
			mActualSize = mSize;
			mData = malloc(mSize);
		}

		if (zeroMem)
			memset(mData, 0, mActualSize);
	}

	void MemoryBlock::free()
	{
		if (mSize <= SMALL_BLOCK)
			getSmallAllocator().free(mData);
		else if (mSize <= MEDIUM_BLOCK)
			getMediumAllocator().free(mData);
		else if (mSize <= LARGE_BLOCK)
			getLargeAllocator().free(mData);
		else
			::free(mData);

		mSize = 0;
		mActualSize = 0;
	}

	TFixedSizeAllocator<MemoryBlock::SMALL_BLOCK, 32>& MemoryBlock::getSmallAllocator()
	{
		static TFixedSizeAllocator<SMALL_BLOCK, 32> allocator;
		return allocator;
	}

	TFixedSizeAllocator<MemoryBlock::MEDIUM_BLOCK, 16>& MemoryBlock::getMediumAllocator()
	{
		static TFixedSizeAllocator<MEDIUM_BLOCK, 16> allocator;
		return allocator;
	}

	TFixedSizeAllocator<MemoryBlock::LARGE_BLOCK, 8>& MemoryBlock::getLargeAllocator()
	{
		static TFixedSizeAllocator<LARGE_BLOCK, 8> allocator;
		return allocator;
	}

}