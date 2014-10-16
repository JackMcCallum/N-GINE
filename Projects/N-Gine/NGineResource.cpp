#include "NGineStable.h"
#include "NGineResource.h"
#include "NGineUtilities.h"

namespace NGine
{
	TPoolAllocator<uint32, 256>& getResourceHandleRefAlloc()
	{
		static TPoolAllocator<uint32, 256> allocator;
		return allocator;
	}

	Resource::Resource(const std::string& name) :
		mName(name),
		mNameHash(Utilities::hash(mName)),
		mPrepared(false),
		mLoaded(false),
		mFailedToLoad(false)
	{

	}

	const std::string& Resource::getName()
	{
		return mName;
	}

	uint32 Resource::getNameHash()
	{
		return mNameHash;
	}

	bool Resource::isPrepared()
	{
		return mPrepared;
	}

	bool Resource::isLoaded()
	{
		return mLoaded;
	}

	bool Resource::failedLoading()
	{
		return mFailedToLoad;
	}

}
