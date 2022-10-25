#ifndef SPHERE
#define SPHERE

#include <array>
#include <optional>

#include "maths.h"
class Ray;
#include "vec3.h"

struct Sphere
{
	using tOptDepth = std::optional<double>;
	using tOptDepths = std::array<tOptDepth, 2>;

	double mRadius;
	Vec3 mCenter;

	Sphere(const double radius, const Vec3& center) : mRadius(radius), mCenter(center) {}

	tOptDepths intersectionDepths(const Ray& ray, const double tMin = 0., const double tMax = infinity) const;
	tOptDepth intersectionDepth(const Ray& ray, const double tMin = 0., const double tMax = infinity) const;

	static void test();
};

#endif
