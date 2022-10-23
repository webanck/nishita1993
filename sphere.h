#ifndef SPHERE
#define SPHERE

#include <optional>

class Ray;
#include "vec3.h"

struct Sphere
{
	double mRadius;
	Vec3 mCenter;

	Sphere(const double radius, const Vec3& center) : mRadius(radius), mCenter(center) {}

	std::optional<double> intersectionDepth(const Ray& ray, const double tMin, const double tMax) const;

	static void test();
};

#endif
