#ifndef CAMERA
#define CAMERA

#include <cassert>

#include "ray.h"
#include "vec3.h"


//Simple pinhole camera.
class Camera
{
	const Vec3 mEye;
	Vec3 mHorizontal; //film x/width vector
	Vec3 mVertical; //film y/height vector
	Vec3 mLowerLeftCorner; //film origin

	//double vfov;
	Vec3 mAxes[3]; //camera referential

public:
	Camera(
		const Vec3& eye,
		const Vec3& at,
		const Vec3& up,
		double vfov,
		double aspectRatio = 1.,
		double focusDistance = 1.
	) : mEye(eye)
	{
		assert(vfov > 0.);
		assert(focusDistance > 0.);

		const double viewportHeight = 2.*std::tan(0.5*vfov);
		const double viewportWidth = aspectRatio * viewportHeight;

		//The depth axis is pointing to the back of the camera.
		mAxes[2] = (eye - at).normalized();
		mAxes[0] = cross(up, mAxes[2]).normalized();
		mAxes[1] = cross(mAxes[2], mAxes[0]);

		mHorizontal = focusDistance * viewportWidth * mAxes[0];
		mVertical = focusDistance * viewportHeight * mAxes[1];
		//The screen is modeled as in front of the camera.
		mLowerLeftCorner = mEye - 0.5*(mHorizontal + mVertical) - focusDistance*mAxes[2];
	}

	Ray buildScreenRay(const double u, const double v) const
	{
		const Vec3 screenPoint = mLowerLeftCorner + u*mHorizontal + v*mVertical;
		const Vec3 direction = (screenPoint - mEye).normalized(); //the screen is modeled as in front of the camera
		return Ray(mEye, direction);
	}
};

#endif
