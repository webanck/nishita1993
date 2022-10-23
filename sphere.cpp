#include "sphere.h"

#include <functional>

#include "camera.h"
#include "image.h"
#include "ray.h"

std::optional<double> Sphere::intersectionDepth(const Ray& ray, const double tMin, const double tMax) const
{
	assert(tMin <= tMax);

	using tReturn = std::optional<double>;

	const Vec3 centerToOrigin = mCenter - ray.mOrigin;
	const double a = 1.;
	const double b = 2.*dot(centerToOrigin, ray.mDirection);
	const double c = centerToOrigin.squaredLength() - mRadius*mRadius;
	const double delta = b*b - 4.*a*c;

	if(delta < 0.)
		return tReturn();

	const double halfDeltaSqrt = 0.5*sqrt(delta);

	const double ta = (-0.5*b + halfDeltaSqrt)/a;
	const double tb = (-0.5*b - halfDeltaSqrt)/a;

	const double depths[2] = {std::min(ta, tb), std::max(ta, tb)};
	if(depths[1] < tMin || depths[0] > tMax)
		return tReturn();

	return tReturn(depths[0] > tMin ? depths[0] : depths[1]);
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

			const std::optional<double> depth = sphere.intersectionDepth(r, closestSphereDepth - 0.01, focusDistance);

			//Depth.
			const double v = depth ? std::abs((*depth - closestSphereDepth) * normalisationFactor) : 0.;
			return Vec3(v, v, v);

			//Normal.
			//return depth ? 0.5*((r(*depth) - sphere.mCenter).normalized() + Vec3(1, 1, 1)) : Vec3(0, 0, 0);
		};

	image.computeFromFunction(sphereDepthFun);
	image.saveAsPPM("sphere_depth_test.ppm");
}

