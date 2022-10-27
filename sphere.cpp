#include "sphere.h"

#include <functional>

#include "camera.h"
#include "image.h"
#include "maths.h"
#include "ray.h"

Sphere::tOptDepths Sphere::intersectionDepths(const Ray& ray, const double tMin, const double tMax) const
{
	assert(tMin <= tMax);

	tOptDepths result;

	const Vec3 centerToOrigin = mCenter - ray.mOrigin;
	const double a = 1.;
	const double b = 2.*dot(centerToOrigin, ray.mDirection);
	const double c = centerToOrigin.squaredLength() - mRadius*mRadius;
	const double delta = sqr(b) - 4.*a*c;

	if(delta < 0.)
		return result;

	const double halfDeltaSqrt = 0.5*sqrt(delta);

	const double ta = (-0.5*b + halfDeltaSqrt)/a;
	const double tb = (-0.5*b - halfDeltaSqrt)/a;

	const double depths[2] = {min(ta, tb), max(ta, tb)};
	if(depths[1] < tMin || depths[0] > tMax)
		return result;

	if(depths[0] >= tMin && depths[1] <= tMax)
{
	result[0].emplace(depths[0]);
		result[1].emplace(depths[1]);
	}
	else
		result[0].emplace(depths[0] > tMin ? depths[0] : depths[1]);
	return result;
}
Sphere::tOptDepth Sphere::intersectionDepth(const Ray& ray, const double tMin, const double tMax) const
{
	return intersectionDepths(ray, tMin, tMax)[0];
}

void Sphere::test()
{
	const Sphere sphere(1., Vec3(0, 0, 0));

	Image image(256, 256);

	const Vec3 eye(0, 0, 3.);
	const Vec3 at(sphere.mCenter);
	const Vec3 up(0, 1, 0);
	const double vfov = 0.7;
	const double aspectRatio = image.getAspectRatio();
	const double focusDistance = (sphere.mCenter - eye).length();
	const Camera camera(eye, at, up, vfov, aspectRatio, focusDistance);

	const double closestSphereDepth = focusDistance - sphere.mRadius;
	const double normalisationFactor = 1./sphere.mRadius;
	std::function<Image::tColor(const uint, const uint)> sphereDepthFun =
		[&sphere, &image, &camera, focusDistance, closestSphereDepth, normalisationFactor](const uint col, const uint row) {
			const Ray r = camera.buildScreenRay(
				(col + 0.5)/image.mWidth,
				(row + 0.5)/image.mHeight
			);

			const tOptDepth depth = sphere.intersectionDepth(r, closestSphereDepth - 0.01, focusDistance);

			//Depth.
			const double v = depth ? abs((*depth - closestSphereDepth) * normalisationFactor) : 0.;
			return Vec3(v, v, v);

			//Normal.
			//return depth ? 0.5*((r(*depth) - sphere.mCenter).normalized() + Vec3(1, 1, 1)) : Vec3(0, 0, 0);
		};

	image.computeFromFunction(sphereDepthFun);
	image.saveAsPPM("sphere_depth_test.ppm");
}

