#include "NGineStable.h"
#include "NGineAABB.h"

namespace NGine
{

	AABB::AABB() :
		mProperties(OUT_OF_DATE),
		mBoundingRadiusSqr(0)
	{

	}

	AABB::AABB(const glm::vec3& min, const glm::vec3& max) :
		mMin(min),
		mMax(max),
		mProperties(OUT_OF_DATE),
		mBoundingRadiusSqr(0)
	{
		_correct();
	}

	const float& AABB::getSquaredBoundingRadius() const
	{
		if (mProperties & RADIUS_DIRTY)
		{
			mBoundingRadiusSqr = glm::length2(getExtents());
			mProperties &= ~RADIUS_DIRTY;
		}
		return mBoundingRadiusSqr;
	}

	const glm::vec3& AABB::getExtents() const
	{
		if (mProperties & EXTENTS_DIRTY)
		{
			mExtents = (mMax - mMin) * 0.5f;
			mProperties &= ~EXTENTS_DIRTY;
		}
		return mExtents;
	}

	const glm::vec3& AABB::getCenter() const
	{
		if (mProperties & CENTER_DIRTY)
		{
			mCenter = (mMax + mMin) * 0.5f;
			mProperties &= ~CENTER_DIRTY;
		}
		return mCenter;
	}

	const glm::vec3& AABB::getMax() const
	{
		return mMax;
	}

	const glm::vec3& AABB::getMin() const
	{
		return mMin;
	}

	bool AABB::isOverlaping(const AABB& aabb) const
	{
		if (aabb.mMin.x < mMax.x &&
			aabb.mMin.y < mMax.y &&
			aabb.mMin.z < mMax.z &&
			aabb.mMax.x > mMin.x &&
			aabb.mMax.y > mMin.y &&
			aabb.mMax.z > mMin.z)
			return true;
		return false;
	}

	bool AABB::isInside(const AABB& aabb) const
	{
		if (aabb.mMin.x > mMin.x &&
			aabb.mMin.y > mMin.y &&
			aabb.mMin.z > mMin.z &&
			aabb.mMax.x < mMax.x &&
			aabb.mMax.y < mMax.y &&
			aabb.mMax.z < mMax.z)
			return true;
		return false;
	}

	void AABB::merge(const AABB& aabb)
	{
		mMax.x = aabb.mMax.x > mMax.x ? aabb.mMax.x : mMax.x;
		mMax.y = aabb.mMax.y > mMax.y ? aabb.mMax.y : mMax.y;
		mMax.z = aabb.mMax.z > mMax.z ? aabb.mMax.z : mMax.z;

		mMin.x = aabb.mMin.x < mMin.x ? aabb.mMin.x : mMin.x;
		mMin.y = aabb.mMin.y < mMin.y ? aabb.mMin.y : mMin.y;
		mMin.z = aabb.mMin.z < mMin.z ? aabb.mMin.z : mMin.z;

		//mMax = glm::max(aabb.mMax, mMax);
		//mMin = glm::min(aabb.mMin, mMin);

		mProperties |= OUT_OF_DATE;
	}

	void AABB::set(const glm::vec3& min, const glm::vec3& max)
	{
		mMax = max;
		mMin = min;
		_correct();

		mProperties |= OUT_OF_DATE;
	}

	void AABB::_correct()
	{
		float temp;
		if (mMax.x < mMin.x)
		{
			temp = mMax.x;
			mMax.x = mMin.x;
			mMin.x = temp;
		}

		if (mMax.y < mMin.y)
		{
			temp = mMax.y;
			mMax.y = mMin.y;
			mMin.y = temp;
		}

		if (mMax.z < mMin.z)
		{
			temp = mMax.z;
			mMax.z = mMin.z;
			mMin.z = temp;
		}
	}

}