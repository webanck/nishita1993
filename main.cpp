#include <iostream>

#include "image.h"

int main(int argc, char** argv)
{
	std::cout << "Hello world!" << std::endl;

	Image(256, 256).saveAsPPM("image.ppm");

	return 0;
}
