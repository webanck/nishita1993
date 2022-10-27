#ifndef ATMOSPHERE
#define ATMOSPHERE

#include "maths.h"
struct Ray;
#include "vec3.h"

struct Atmosphere
{
	static constexpr double mOuterRadius = 6429000; //in meters
	static constexpr double mInnerRadius = 6360000; //in meters
	static constexpr double mAirDensityScaleHeight = 7994; //in meters
	static constexpr double mAerosolsDensityScaleHeight = 1200; //in meters

	static const Vec3 mAttenuationAirRGB; //in meters^-1
	static constexpr double mAttenuationAerosols = 10.e-6 * 21.; //in meters^-1

	static const Vec3 mSunLightIntensityRGB;


	static Vec3 singleScatteredLight(const Ray& incidentRay);

	static double rayleighPhase(const double scatteringAngle)
	{
		assert(scatteringAngle >= 0. && scatteringAngle <= pi);
		return 0.25*3.*(1. + sqr(std::cos(scatteringAngle)));
		//todo: missing K part maybe not in phase
	}

	static double getAltitude(const Vec3& p)
	{
		return p.length() - mInnerRadius;
	}
	static double getAirDensity(const double altitude)
	{
		if(altitude > mOuterRadius || altitude < mInnerRadius) return 0.;
		return std::exp(-altitude/mAirDensityScaleHeight);
	}
	static double getAerosolsDensity(const double altitude)
	{
		if(altitude > mOuterRadius || altitude < mInnerRadius) return 0.;
		return std::exp(-altitude/mAerosolsDensityScaleHeight);
	}

	static Vec3 getSunTransmittance(const double altitude, const double zenitalAngle);

	static void test();
};

#endif
