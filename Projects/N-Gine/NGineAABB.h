#ifndef __NGINE_AABB_H_
#define __NGINE_AABB_H_

#include "NGineCommon.h"
#include <glm/glm.hpp>

namespace NGine
{
	class AABB
	{
	private:
		enum Properties
		{
			CENTER_DIRTY = (1 << 0),
			EXTENTS_DIRTY = (1 << 1),
			RADIUS_DIRTY = (1 << 2),
			OUT_OF_DATE = CENTER_DIRTY | EXTENTS_DIRTY | RADIUS_DIRTY,
		};

	public:
		AABB();
		AABB(const glm::vec3& min, const glm::vec3& max);

		// Merge this aabb with the supplied aabb
		void merge(const AABB& aabb);

		// Set this aabb
		void set(const glm::vec3& min, const glm::vec3& max);

		// Return true if the supplied aabb is inside this
		bool isInside(const AABB& aabb) const;

		// Return true if the supplied aabb is overlapping with this
		bool isOverlaping(const AABB& aabb) const;

		// Get the min position
		const glm::vec3& getMin() const;

		// Get the max position
		const glm::vec3& getMax() const;

		// Return the center of the aabb
		const glm::vec3& getCenter() const;

		// Return the extents of the aabb
		const glm::vec3& getExtents() const;
		
		// Return the squared bounding radius of the box
		const float& getSquaredBoundingRadius() const;

	private:
		// Correct the bounding box if its inverted on an axis
		void _correct();

	private:
		glm::vec3 mMin;
		glm::vec3 mMax;
		mutable glm::vec3 mCenter;
		mutable glm::vec3 mExtents;
		mutable float mBoundingRadiusSqr;
		mutable uint32 mProperties;
	};
}

#endif // !__NGINE_AABB_H_
