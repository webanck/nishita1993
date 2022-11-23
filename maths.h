#ifndef MATHS
#define MATHS

#include <cmath>
#include <limits>

const double pi = std::acos(-1.);
constexpr double infinity = std::numeric_limits<double>::infinity();

using std::min;
using std::max;
using std::abs;

inline constexpr double sqr(const double x) { return x*x; }

#endif
