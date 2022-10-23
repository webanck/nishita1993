#ifndef VEC3
#define VEC3

#include <cassert>
#include <valarray>

class Vec3
{
	std::valarray<double> mCoords;

	Vec3(const std::valarray<double> coords) : mCoords(coords) { assert(coords.size() == 3); }
public:
	Vec3() : mCoords{0., 0., 0.} {}
	Vec3(const double x, const double y, const double z) : mCoords{x, y, z} {}

	double x() const { return mCoords[0]; }
	double y() const { return mCoords[1]; }
	double z() const { return mCoords[2]; }

	//todo: missing operators
	Vec3 operator+(const Vec3& b) const { return Vec3(mCoords + b.mCoords); }
	Vec3 operator-(const Vec3& b) const { return Vec3(mCoords - b.mCoords); }
	friend Vec3 operator*(const double s, const Vec3& v);

	friend double dot(const Vec3& a, const Vec3& b);
	friend Vec3 cross(const Vec3& a, const Vec3& b);

	Vec3 normalized() const
	{
		const double l = length();
		assert(l > 0.0001);
		return (1./l) * *this;
	}

	double squaredLength() const { return (mCoords*mCoords).sum(); }
	double length() const { return std::sqrt(squaredLength()); }
};

inline Vec3 operator*(const double s, const Vec3& v)
{
	return Vec3(s * v.mCoords);
}
inline double dot(const Vec3& a, const Vec3& b)
{
	return (a.mCoords * b.mCoords).sum();
}
inline Vec3 cross(const Vec3& a, const Vec3& b)
{
	return Vec3(a.mCoords.cshift(1)*b.mCoords.cshift(2) - a.mCoords.cshift(2)*b.mCoords.cshift(1));
}

#endif
