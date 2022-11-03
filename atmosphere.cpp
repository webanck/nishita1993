#include "atmosphere.h"

#include "camera.h"
#include "image.h"
#include "sphere.h"

const Vec3 Atmosphere::mAttenuationAirRGB = 1.e-6 * Vec3(3.8, 13.5, 33.1);
const Vec3 Atmosphere::mSunLightIntensityRGB = 10./255.*Vec3(255, 240, 233); //RGB approximation of 5800 Kelvin degrees light
const Vec3 Atmosphere::mSunLightDirection = -Vec3(1.).normalized();

//Ray is expressed in the Earth centered referential.
Vec3 Atmosphere::singleScatteredLight(const Ray& incidentRay)
{
	assert(incidentRay.mOrigin.squaredLength() > sqr(mInnerRadius));
	assert(incidentRay.mOrigin.squaredLength() > sqr(mOuterRadius)); //not mandatory but this fits the "viewed from space" case

	Vec3 light(0.);

	const Sphere::tOptDepths outerOptDepths = Sphere(mOuterRadius, Vec3(0.)).intersectionDepths(incidentRay);
	if(!outerOptDepths[0]) return light;

	const double tMin = *(outerOptDepths[0]);
	assert(outerOptDepths[1]);

	const Sphere::tOptDepth innerOptDepth = Sphere(mInnerRadius, Vec3(0.)).intersectionDepth(incidentRay);
	const double tMax = innerOptDepth ? *innerOptDepth : *(outerOptDepths[1]);
	const double middleAltitude = getAltitude(incidentRay(0.5*(tMin + tMax)));
	assert(middleAltitude >= 0. && middleAltitude <= mOuterRadius - mInnerRadius);

	const double scatteringAngleCos = -dot(incidentRay.mDirection, mSunLightDirection);
	const double airPhase = rayleighPhase(scatteringAngleCos);
	const double aerosolsPhase = cornettePhase(scatteringAngleCos);

	//Coarse ray marching
	{
		const double l = tMax - tMin;
		assert(l > 0.);
		const uint iMax = 10u;
		const double stepSize = l/iMax;

		Vec3 opticalDepth(0, 0, 0);
		for(uint i = 0u; i < iMax; i++)
		{
			const double t = tMin + (i + 0.5)*stepSize;
			const Vec3 p = incidentRay(t);

			const double h = getAltitude(p);
			assert(h >= 0. && h < mOuterRadius - mInnerRadius);
			const double airDensity = getAirDensity(h);
			assert(airDensity > 0.);
			const double aerosolsDensity = getAerosolsDensity(h);
			assert(aerosolsDensity > 0.);

			opticalDepth += stepSize * (airDensity * mAttenuationAirRGB + aerosolsDensity * mAttenuationAerosols);

			if(isInEarthShadow(p)) continue;

			const Vec3 transmittance = exp(-(opticalDepth + getSunOpticalDepth(p)));
			const Vec3 airScattering = airPhase * airDensity * mAlbedoAir * mAttenuationAirRGB;
			const double aerosolsScattering = aerosolsPhase * aerosolsDensity * mAlbedoAerosols * mAttenuationAerosols;
			light += stepSize * (airScattering + Vec3(aerosolsScattering)) * transmittance * mSunLightIntensityRGB;
		}
	}

	return light;
}

bool Atmosphere::isInEarthShadow(const Vec3& p)
{
	return Sphere(mInnerRadius, Vec3(0.)).intersectionDepth(getSunRay(p)).has_value();
}
Vec3 Atmosphere::getSunOpticalDepth(const Vec3& p)
{
	assert(p.squaredLength() > sqr(mInnerRadius));
	assert(p.squaredLength() < sqr(mOuterRadius));

	assert(!isInEarthShadow(p));

	//const double zenithalAngle = -dot(p.normalized(), mSunLightDirection);

	const Ray sunRay = getSunRay(p);
	const Sphere::tOptDepth optMaxDepth = Sphere(mOuterRadius, Vec3(0.)).intersectionDepth(sunRay);
	assert(optMaxDepth);

	//Coarse ray marching
	const double l = *optMaxDepth;
	const uint iMax = 10u;
	const double stepSize = l/iMax;

	double airDensitySum = 0.;
	double aerosolsDensitySum = 0.;
	for(uint i = 0u; i < iMax; i++)
	{
		const double t = (i + 0.5)*stepSize;
		const Vec3 p = sunRay(t);

		const double h = getAltitude(p);
		const double airDensity = getAirDensity(h);
		assert(airDensity > 0.);
		const double aerosolsDensity = getAerosolsDensity(h);
		assert(aerosolsDensity > 0.);

		airDensitySum += airDensity;
		aerosolsDensitySum += aerosolsDensity;
	}
	return stepSize * (airDensitySum*mAttenuationAirRGB + Vec3(aerosolsDensitySum*mAttenuationAerosols));
}


void Atmosphere::test()
{
	const Sphere sphere(1., Vec3(0.));

	Image image(512, 512);

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

			return singleScatteredLight(earthRay);
		};

	image.computeFromFunction(sphereDepthFun);
	image.saveAsPPM("atmosphere_test.ppm");
}
