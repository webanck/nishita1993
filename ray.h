#ifndef RAY
#define RAY

#include <cassert>

#include "vec3.h"

struct Ray
{
	Ray(const Vec3& origin, const Vec3& direction) : mOrigin(origin), mDirection(direction)
	{
		assert(std::abs(mDirection.squaredLength() - 1.) < 0.001);
	}
	Vec3 operator()(const double t) { return mOrigin + t * mDirection; }

	Vec3 mOrigin;
	Vec3 mDirection;
};

#endif
