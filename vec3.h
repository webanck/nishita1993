#ifndef VEC3
#define VEC3

#include <cassert>
#include <ostream>
#include <valarray>


template<typename T>
T add(T a, T b) { return a + b; }
template<typename T>
T sub(T a, T b) { return a - b; }
template<typename T>
T mul(T a, T b) { return a * b; }
template<typename T>
T div(T a, T b) { return a / b; }
template<typename T>
T neg(T a) { return -a; }

class Vec3
{
	std::array<double, 3> mCoords;

public:
	Vec3() : mCoords{0., 0., 0.} {}
	Vec3(const double x, const double y, const double z) : mCoords{x, y, z} {}
	Vec3(const double x) : mCoords{x, x, x} {}

	double x() const { return mCoords[0]; }
	double y() const { return mCoords[1]; }
	double z() const { return mCoords[2]; }

	template<double (*T)(double, double)>
	Vec3 binaryOperation(const Vec3& b) const
	{
		return Vec3(
			T(x(), b.x()),
			T(y(), b.y()),
			T(z(), b.z())
		);
	}
	Vec3 shift(const uint i) const
	{
		assert(i < 3u);
		return Vec3(
			mCoords[i],
			mCoords[(i + 1)%3],
			mCoords[(i + 2)%3]
		);
	}
	template<double (*T)(double)>
	Vec3 map() const
	{
		return Vec3(T(x()), T(y()), T(z()));
	}
	template<double (*T)(double, double)>
	double reduce() const
	{
		return T(T(x(), y()), z());
	}

	//todo: missing operators
	Vec3 operator+(const Vec3& b) const { return binaryOperation<add<double>>(b); }
	Vec3 operator-(const Vec3& b) const { return binaryOperation<sub<double>>(b); }
	Vec3 operator*(const Vec3& b) const { return binaryOperation<mul<double>>(b); }
	Vec3 operator-() const { return map<neg<double>>(); }
	friend Vec3 operator*(const double s, const Vec3& v);

	void operator+=(const Vec3& v) { operator=(operator+(v)); }

	Vec3 normalized() const
	{
		const double l = length();
		assert(l > 0.0001);
		return (1./l) * *this;
	}

	double squaredLength() const { return operator*(*this).reduce<add<double>>(); }
	double length() const { return std::sqrt(squaredLength()); }
};

inline Vec3 operator*(const double s, const Vec3& v)
{
	return Vec3(s * v.x(), s * v.y(), s * v.z());
}
inline Vec3 exp(const Vec3& v)
{
	return v.map<exp>();
}
inline double dot(const Vec3& a, const Vec3& b)
{
	return (a * b).reduce<add<double>>();
}
inline Vec3 cross(const Vec3& a, const Vec3& b)
{
	return Vec3(a.shift(1)*b.shift(2) - a.shift(2)*b.shift(1));
}

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')';
}

#endif
