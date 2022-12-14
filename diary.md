# 12/10/2022
13h15-14h15

- Basic setup : makefile, main, journal et dépôt git.
- PPM image support.

# 14/10/2022
13h30-14h30

- Ray-sphere intersection implementation (to test for compilation and correctness).

# 23/10/2022
15h30-15h45, 16h-18h, 22h30-23h30

- Compilation and makefile fixes.
- Pinhole camera and ray tracing for ray-sphere intersection depth testing.
- Separated compilation with automated headers dependency.

# 25/10/2022
7h-9h

- Multiple ray-sphere intersections.
- Beginning of the atmospheric single scattering implementation.

# 27/10/2022
22h-23h30

- Coarse single scattering test case.
  There is an issue with the ray-atmosphere intersection; the intersection interval is often out of the atmosphere shell.
  The issue is probably coming from the ray-sphere intersection function.

# 31/10/2022
20h-23h30

- Ray--sphere intersecion fix: there was a wrongly reversed vector in the computation.
- Attenuations scale fix: order of $10^{-6}$ is written `1.e-6` in C++.
- First image with coarse ray-marching of the atmosphere:
  - 10 steps along direct ray,
  - 10 steps along shadow/sun ray with Earth's surface shadow,
  - air and aerosols densities and phase functions accounted for.
- The next step is optimisation:
  - usage of OpenMP,
  - profiling on the test case,
  - optical depth tabulation considering Earth is spherical,
  - ray-marching with a step size increasing exponentially with altitude.

# 03/11/2022
23h-23h45

- Copy of the repository on GitHub, and description.
- Use of OpenMP for multithreaded rendering.

# 04/11/2022
8h45-9h

- A bit more info in the README.

# 15/11/2022
13h-14h

- Ray-marching at a step of around 10km instead of a tenth of the ray length.
- Compilation parameters for profiling.

# 17/11/2022
22h-23h

- Profiling with `gprof` estimates exclusive computation time at :
  - 60% in `getSunOpticalDepth`,
  - 14% in `singleScatteredLight`,
  - and only 5% in `Sphere::intersectionDepths`.
  However, the former percentages probably include computations from inlined functions such as `getAirDensity`, `getAerosolsDensity` and `getAltitude`.
  Indeed, they don't appear in the report even though they should be the most called functions.
- Adding `__attribute__ ((noinline))` in front of a function prototype avoids it from being inlined.
  Doing so for `getAirDensity`, `getAerosolsDensity` and `getAltitude`, `gprof` exclusive computation times become :
  - 38% in `getSunOpticalDepth`,
  - 26% in `singleScatteredLight`,
  - 10% in `getAltitude`,
  - 6% in `getAirDensity` and `getAerosolsDensity` each,
  - and only 3% in `Sphere::intersectionDepths` and `Sphere::intersectionDepth` each.

# 23/11/2022
17h30-18h30, 22h-23h30

- Profiling with `perf` strangely reports a lot of calls to `__nss_database_lookup2` in `libc-2.31.so`.
  It seems the executable doesn't get linked to the shared libraries with debug symbols, even though they are available on my OS.
  Compiling with `-static` circumvent this issue.
- Profiling with `perf` shows 64% of the cycles in `__mcount_internal`.
  This function is linked to profiling counters, the feature enabled by the `-pg` compiling option.
  Because `perf` doesn't need that option, I'm getting rid of it.
- Profiling with `perf` now puts the cycles distribution to:
  - 32% in `_int_free`,
  - 22% in `malloc`,
  - 10% in `free`,
  - 7% in `__ieee754_exp_fma`,
  - and then 6% in `Atmosphere::getSunOpticalDepth` and 5% in `Atmosphere::getAltitude`.
  I guess those allocations related calls are done by `std::valarray`.
- I replaced `std::valarray` by `std::array`.
  Profiling with `perf`, cycles are now:
  - 34% in `__ieee754_exp_fma`,
  - 21% in `Atmosphere::getAltitude`,
  - and 10% in `Atmosphere::getSunOpticalDepth`.
