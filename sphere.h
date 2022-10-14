#include <optional>

#include "vec3.h"

struct Sphere
{
	double mRadius;
	Vec3 mCenter;

	std::optional<Vec3> intersection(const Ray& ray, const double tMin, const double tMax)
	{
		assert(tMin <= tMax);

		using tReturn = std::optional<Vec3>;

		const Vec3 centerToOrigin = mCenter - ray.mOrigin;
		const double a = 1.;
		const double b = 2.*dot(centerToOrigin, ray.mDirection());
		const double c = centerToOrigin.squaredLength() - mRadius*mRadius;
		const double delta = b*b - 4.*a*c;

		if(delta < 0.)
			return tReturn();

		const double halfDeltaSqrt = 0.5*sqrt(delta);

		const double ta = (-0.5*b + halfDeltaSqrt)/a;
		const double tb = (-0.5*b - halfDeltaSqrt)/a;

		const double depths[2] = {std::min(ta, tb), std::max(ta, tb)};
		if(depths[1] < tMin || depths[0] > tMax)
			return tReturn;

		return tReturn(depths[0] > tMin ? depths[0] : depths[1]);
	}
}
