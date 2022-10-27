#include "atmosphere.h"

#include "camera.h"
#include "image.h"
#include "ray.h"
#include "sphere.h"

const Vec3 Atmosphere::mAttenuationAirRGB = 10.e-6 * Vec3(3.8, 13.5, 33.1);
const Vec3 Atmosphere::mSunLightIntensityRGB = 0.01 * Vec3(1, 1, 1);

//Ray is expressed in the Earth centered referential.
Vec3 Atmosphere::singleScatteredLight(const Ray& incidentRay)
{
	assert(incidentRay.mOrigin.squaredLength() > sqr(mInnerRadius));
	assert(incidentRay.mOrigin.squaredLength() > sqr(mOuterRadius)); //not mandatory but this fits the "viewed from space" case

	Vec3 light(0, 0, 0);

	const Sphere::tOptDepths outerOptDepths = Sphere(mOuterRadius, Vec3(0, 0, 0)).intersectionDepths(incidentRay);
	if(!outerOptDepths[0]) return light;

	const double tMin = *(outerOptDepths[0]);
	assert(outerOptDepths[1]);

	const Sphere::tOptDepth innerOptDepth = Sphere(mInnerRadius, Vec3(0, 0, 0)).intersectionDepth(incidentRay);
	const double tMax = innerOptDepth ? *innerOptDepth : *(outerOptDepths[1]);
	const double middleAltitude = getAltitude(incidentRay(0.5*(tMin + tMax)));
	std::cerr << "tMinAltitude: " << getAltitude(incidentRay(tMin)) << std::endl;
	std::cerr << "tMaxAltitude: " << getAltitude(incidentRay(tMax)) << std::endl;
	std::cerr << "middleAltitude: " << middleAltitude << std::endl;
	assert(middleAltitude >= 0. && middleAltitude <= mOuterRadius - mInnerRadius);

	//Coarse ray marching
	{
		const double l = tMax - tMin;
		const uint iMax = 10u;
		const double stepSize = l/iMax;

		Vec3 opticalDepth(0, 0, 0);
		for(uint i = 0u; i < iMax; i++)
		{
			const double t = tMin + i*stepSize;
			const Vec3 p = incidentRay(t);

			const double h = getAltitude(p);
			std::cerr << "altitude: " << h << std::endl;
			assert(h >= 0. && h < mOuterRadius - mInnerRadius);
			const double airDensity = getAirDensity(h);
			std::cerr << "airDensity: " << airDensity << std::endl;
			//const double aerosolsDensity = getAerosolsDensity(h);

			opticalDepth += stepSize * (airDensity * mAttenuationAirRGB /*+aerosolsDensity * mAttenuationAerosols*/);

			const Vec3 primaryTransmittance = exp(-opticalDepth);
			const Vec3 secondaryTransmittance = Vec3(1, 1, 1);//getSunTransmittance(h, );
			std::cerr << "transmittance: " << (primaryTransmittance * secondaryTransmittance).x() << std::endl;

			light += stepSize * primaryTransmittance * secondaryTransmittance * mSunLightIntensityRGB;
		}
	}

	return light;
}

Vec3 Atmosphere::getSunTransmittance(const double altitude, const double zenitalAngle)
{
	//Coarse ray marching
	return Vec3(1, 1, 1);
}


void Atmosphere::test()
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

	const double closestSphereDepth = focusDistance - sphere.mRadius - 0.01;
	std::function<Image::tColor(const uint, const uint)> sphereDepthFun =
		[&sphere, &image, &camera, focusDistance, closestSphereDepth](const uint col, const uint row) {
			const Ray r = camera.buildScreenRay(
				(col + 0.5)/image.mWidth,
				(row + 0.5)/image.mHeight
			);

			const Ray earthRay(mOuterRadius * r(closestSphereDepth), r.mDirection);
			//std::cerr << "ray origin: " << r(closestSphereDepth) << std::endl;
			//std::cerr << "ray direction: " << r.mDirection << std::endl;

			return singleScatteredLight(earthRay);
		};

	image.computeFromFunction(sphereDepthFun);
	image.saveAsPPM("atmosphere_test.ppm");
}
