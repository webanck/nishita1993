#include <valarray>

class Vec3
{
	std::valarray<double> mCoords;

public:
	Vec3() : mCoords{0., 0., 0.} {}
	Vec3(const double x, const double y, const double z) : mCoords{x, y, z} {}

	double x() const { return mCoords[0]; }
	double y() const { return mCoords[1]; }
	double z() const { return mCoords[2]; }
};
