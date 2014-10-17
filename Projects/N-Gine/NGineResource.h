#ifndef __NGINE_RESOURCE_H_
#define __NGINE_RESOURCE_H_

#include <memory>
#include <mutex>
#include "NGineCommon.h"
#include "NGineFixedSizeAllocator.h"

namespace NGine
{
	TFixedTypeAllocator<uint32, 256>& getResourceHandleRefAlloc();

	template<typename T>
	class TResourceHandle
	{
	public:
		TResourceHandle() :
			mPointer(nullptr),
			mOwners(nullptr)
		{
		}

		TResourceHandle(T* ptr) :
			mPointer(ptr),
			mOwners(getResourceHandleRefAlloc().allocate())
		{
			(*mOwners) = 1;
		}

		~TResourceHandle()
		{
			if (mOwners && (--(*mOwners)) == 0)
				getResourceHandleRefAlloc().free(mOwners);
		}

		TResourceHandle(const TResourceHandle<T>& other) :
			mPointer(other.mPointer),
			mOwners(other.mOwners)
		{
			if (mOwners) ++(*mOwners);
		}

		TResourceHandle& operator = (const TResourceHandle<T>& other)
		{
			if (mOwners && (--(*mOwners)) == 0)
				getResourceHandleRefAlloc().free(mOwners);

			mPointer = other.mPointer;
			mOwners = other.mOwners;
			if (mOwners) ++(*mOwners);
			return *this;
		}

		inline bool operator == (const TResourceHandle<T>& other) const { return mPointer == other.mPointer; }
		inline bool operator != (const TResourceHandle<T>& other) const { return mPointer != other.mPointer; }
		inline operator bool() const { return (mPointer != nullptr); }
		inline T* operator -> () const { return mPointer; }
		inline T& operator * () const { return *mPointer; }
		inline uint32 owners() const { return *mOwners; }
		inline T* get() const { return mPointer; }

	private:
		T* mPointer;
		uint32* mOwners;
	};

#define RESOURCE_PTR(type) \
	class type; \
	typedef TResourceHandle<type> type##Ptr;

	RESOURCE_PTR(Texture);
	RESOURCE_PTR(Mesh);
	RESOURCE_PTR(Shader);
	RESOURCE_PTR(Material);
	class ResourceManager;

	class Resource
	{
	public:
		Resource(const std::string& name);
		
		const std::string& getName();
		uint32 getNameHash();

		bool isPrepared();
		bool isLoaded();
		bool failedLoading();

	protected:
		std::string mName;
		uint32 mNameHash;

		bool mPrepared;
		bool mLoaded;

		bool mFailedToLoad;
	};

}

#endif // !__NGINE_RESOURCE_H_
