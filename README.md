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
- Attenuations scale fix: order of $10^-6$ is noted `1.e-6` in C++.
- First image with coarse ray-marching of the atmosphere :
  - 10 steps along direct ray,
  - 10 steps along shadow/sun ray with Earth's surface shadow,
  - air and aerosols densities and phase functions accounted for.
  The next step is optimisation:
  - usage of OpenMP,
  - profiling on the test case,
  - optical depth tabulation considering Earth is spherical,
  - ray-marching with a step size increasing exponentially with altitude.
