#ifndef IMAGE
#define IMAGE

#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "types.h"
#include "vec3.h"

struct Image
{
	using tColor = Vec3;

	uint mWidth;
	uint mHeight;
	std::vector<tColor> mColors;

	Image(const uint width, const uint height) : mWidth(width), mHeight(height), mColors(mWidth*mHeight) {}

	uint pixelIndex(const uint col, const uint row) const
	{
		assert(col < mWidth);
		assert(row < mHeight);

		//Row major.
		return row*mWidth + col;
	}
	const tColor& operator()(const uint col, const uint row) const
	{
		return mColors[pixelIndex(col, row)];
	}
	tColor& operator()(const uint col, const uint row)
	{
		return mColors[pixelIndex(col, row)];
	}

	void saveAsPPM(const std::string& path) const
	{
		std::cerr << "Saving image to file '" << path << "'." << std::endl;

		std::ofstream file;
		file.open(path);
		writePPM(file);
		file.close();
	}

	double getAspectRatio() const { return static_cast<double>(mWidth)/mHeight; }

	void computeFromFunction(const std::function<tColor(const uint, const uint)>& fun)
	{
		#pragma omp parallel for schedule(dynamic, 1) collapse(2)
		for(uint col = 0u; col < mWidth; col++)
		for(uint row = 0u; row < mHeight; row++)
			operator()(col, row) = fun(col, row);
	}

	static void test()
	{
		Image(256, 256).saveAsPPM("black_256x256.ppm");
	}

private:
	static void writeColorPPM(std::ostream& out, const tColor& color)
	{
		assert(color.x() >= 0.);
		assert(color.y() >= 0.);
		assert(color.z() >= 0.);

		out
			<< static_cast<int>(256 * color.x()) << " "
			<< static_cast<int>(256 * color.y()) << " "
			<< static_cast<int>(256 * color.z())
		<< "\n";
	}
	void writePPM(std::ostream& out) const
	{
		out
			<< "P3\n"
			<< mWidth << " " << mHeight << "\n"
			<< "255\n"
		;
		
		for(uint row = mHeight - 1u; row < mHeight; row--)
		for(uint col = 0u; col < mWidth; col++)
			writeColorPPM(out, operator()(col, row));
	}
};

#endif
