#include "atmosphere.h"

#include "ray.h"
#include "sphere.h"

const Vec3 Atmosphere::mBetaAirRGB = 10.e-6 * Vec3(3.8, 13.5, 33.1);
const Vec3 Atmosphere::mSunLightIntensity = Vec3(1, 1, 1);

//Ray is expressed in the Earth centered referential.
Vec3 Atmosphere::singleScatteredLight(const Ray& incidentRay)
{
	assert(incidentRay.mOrigin.squaredLength() > mInnerRadius);
	assert(incidentRay.mOrigin.squaredLength() > mOuterRadius); //not mandatory but this fits the "viewed from space" case

	Vec3 light(0, 0, 0);

	const Sphere::tOptDepths outerOptDepths = Sphere(mOuterRadius, Vec3(0, 0, 0)).intersectionDepths(incidentRay);
	if(!outerOptDepths[0]) return light;

	const double tMin = *(outerOptDepths[0]);
	assert(outerOptDepths[1]);

	const Sphere::tOptDepth innerOptDepth = Sphere(mInnerRadius, Vec3(0, 0, 0)).intersectionDepth(incidentRay);
	const double tMax = innerOptDepth ? *innerOptDepth : *(outerOptDepths[1]);

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
			const double airDensity = getAirDensity(h);
			//const double aerosolsDensity = getAerosolsDensity(h);

			const Vec3 primaryTransmittance = exp(-opticalDepth);
			const Vec3 secondaryTransmittance = getTransmittance(
			light += stepSize * std::exp(-opticalDepth) * getTransmittance(Ray())
		}
	}

	return light;
}

Vec3 Atmosphere::getSunTransmittance(const double altitude)
{
	//Coarse ray marching
}
