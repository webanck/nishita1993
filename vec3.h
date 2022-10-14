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

	//todo: missing operators
	friend double dot(const Vec3& a, const Vec3& b);

	double squaredLength() const { return (mCoords*mCoords).sum(); }
	double length() const { return std::sqrt(squaredLength()); }
};

double dot(const Vec3& a, const Vec3& b)
{
	return (a.mCoords * b.mCoords).sum();
}
