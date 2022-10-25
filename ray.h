#ifndef RAY
#define RAY

#include <cassert>

#include "vec3.h"

struct Ray
{
	const Vec3 mOrigin;
	const Vec3 mDirection;

	Ray(const Vec3& origin, const Vec3& direction) : mOrigin(origin), mDirection(direction)
	{
		assert(abs(mDirection.squaredLength() - 1.) < 0.001);
	}
	Vec3 operator()(const double t) const { return mOrigin + t * mDirection; }
};

#endif
