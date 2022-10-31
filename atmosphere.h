#ifndef ATMOSPHERE
#define ATMOSPHERE

#include "maths.h"
#include "ray.h"
#include "vec3.h"

struct Atmosphere
{
	static constexpr double mOuterRadius = 6429000; //in meters
	static constexpr double mInnerRadius = 6360000; //in meters
	static constexpr double mAirDensityScaleHeight = 7994; //in meters
	static constexpr double mAerosolsDensityScaleHeight = 1200; //in meters

	static const Vec3 mAttenuationAirRGB; //in meters^-1 in each channel
	static constexpr double mAttenuationAerosols = 1.e-6 * 21.; //in meters^-1
	
	static constexpr double mAlbedoAir = 0.25/pi; //eqs 1 and 3 in Nishita1993
	static constexpr double mAlbedoAerosols = 1.;

	static const Vec3 mSunLightIntensityRGB;
	static const Vec3 mSunLightDirection;


	static Vec3 singleScatteredLight(const Ray& incidentRay);

	static double rayleighPhase(const double scatteringAngleCos)
	{
		assert(scatteringAngleCos >= -1. && scatteringAngleCos <= 1.);
		return 0.25*3.*(1. + sqr(scatteringAngleCos));
	}
	static double cornettePhase(const double scatteringAngleCos)
	{
		assert(scatteringAngleCos >= -1. && scatteringAngleCos <= 1.);

		//see eq 5 of Nishita1993
		static constexpr double u = 0.7; //in [0.7, 0.85]
		static constexpr double x = 5./9.*u + 125./729.*u*u*u + sqrt(64./27. - 325./243.*u*u + 1250./2187.*u*u*u*u);
		static constexpr double g = 5./9.*u - (4./3. - 25./81.*sqr(u))*pow(x, -1./3.) + pow(x, 1./3.);

		return 1.5*(1. - g*g)*(1. + sqr(scatteringAngleCos)) / ((2. + g*g)*pow(1. + g*g - 2.*g*scatteringAngleCos, 1.5));
	}

	static double getAltitude(const Vec3& p)
	{
		return p.length() - mInnerRadius;
	}
	static double getAirDensity(const double altitude)
	{
		static const double atmosphereHeight = mOuterRadius - mInnerRadius;
		if(altitude < 0. || altitude > atmosphereHeight) return 0.;
		return std::exp(-altitude/mAirDensityScaleHeight);
	}
	static double getAerosolsDensity(const double altitude)
	{
		static const double atmosphereHeight = mOuterRadius - mInnerRadius;
		if(altitude < 0. || altitude > atmosphereHeight) return 0.;
		return std::exp(-altitude/mAerosolsDensityScaleHeight);
	}

	static Ray getSunRay(const Vec3& p)
	{
		return Ray(p, -mSunLightDirection);
	}
	static bool isInEarthShadow(const Vec3& p);
	static Vec3 getSunOpticalDepth(const Vec3& p);

	static void test();
};

#endif
