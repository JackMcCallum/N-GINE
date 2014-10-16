#ifndef __NGINE_MEMORY_BLOCK_H_
#define __NGINE_MEMORY_BLOCK_H_

#include "NGineCommon.h"
#include "NGineFixedSizeAllocator.h"

namespace NGine
{
	class MemoryBlock
	{
	public:
		MemoryBlock();

		// Construct and allocate
		MemoryBlock(size_t size, bool zeroMem = false);

		void free();

		// Get the size
		inline size_t size() const { return mSize; }

		// Get the actual block size that was allocated for this
		// With smaller blocks this may be bigger
		inline size_t actualSize() const { return mActualSize; }

		// Pointer to the start of the block
		inline void* data() const { return mData; }

		// Helper method for using the memory as an array
		template<typename T>
		T& idx(size_t idx) { return ((T*)mData)[idx]; }

	private:
		enum BlockSize
		{
			SMALL_BLOCK = 1024,
			MEDIUM_BLOCK = 1024 * 8,
			LARGE_BLOCK = 1024 * 64,
		};

		static TFixedSizeAllocator<SMALL_BLOCK, 32>& getSmallAllocator();
		static TFixedSizeAllocator<MEDIUM_BLOCK, 16>& getMediumAllocator();
		static TFixedSizeAllocator<LARGE_BLOCK, 8>& getLargeAllocator();
		
	private:
		size_t mSize;
		size_t mActualSize;
		void* mData;
	};
}


#endif // !__NGINE_MEMORY_BLOCK_H_
