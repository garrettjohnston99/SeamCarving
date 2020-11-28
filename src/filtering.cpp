/*
    CS 89/189 Computational Aspects of Digital Photography C++ basecode.

    Adapted from MIT's 6.815/6.865 basecode, written and designed by:
        Frédo Durand
        Katherine L. Bouman
        Gaurav Chaurasia
        Adrian Vasile Dalca
        Neal Wadhwa

    With additions & modifications by
        Wojciech Jarosz
    for Dartmouth's CS 89/189.
*/

// filtering.cpp
// Assignment 4

#include "filtering.h"
#include <math.h>

using namespace std;

/**************************************************************
 //            IMAGE CONVOLUTION AND FILTERING               //
 *************************************************************/


// uses a Sobel kernel to compute the horizontal and vertical
// components of the gradient of an image and returns the gradient magnitude.
FloatImage gradientMagnitude(const FloatImage &im, bool clamp)
{
	// sobel filtering in x direction
	Filter sobelX(3, 3);
	sobelX(0, 0) = -1.0;
	sobelX(1, 0) = 0.0;
	sobelX(2, 0) = 1.0;
	sobelX(0, 1) = -2.0;
	sobelX(1, 1) = 0.0;
	sobelX(2, 1) = 2.0;
	sobelX(0, 2) = -1.0;
	sobelX(1, 2) = 0.0;
	sobelX(2, 2) = 1.0;

	FloatImage imSobelX = sobelX.Convolve(im, clamp);

	// sobel filtering in y direction
	Filter sobelY(3, 3);
	sobelY(0, 0) = -1.0;
	sobelY(1, 0) = -2.0;
	sobelY(2, 0) = -1.0;
	sobelY(0, 1) = 0.0;
	sobelY(1, 1) = 0.0;
	sobelY(2, 1) = 0.0;
	sobelY(0, 2) = 1.0;
	sobelY(1, 2) = 2.0;
	sobelY(2, 2) = 1.0;

	FloatImage imSobelY = sobelY.Convolve(im, clamp);

	// squared magnitude
	FloatImage magnitude = imSobelX * imSobelX + imSobelY * imSobelY;

	// take the square root
	for (int i = 0; i < magnitude.size(); i++)
	{
		magnitude(i) = sqrt(magnitude(i));
	}

	return magnitude;
}

/**************************************************************
 //                 FILTER CLASS FUNCTIONS                  //
 *************************************************************/


// write a convolution function for the filter class
FloatImage Filter::Convolve(const FloatImage &im, bool clamp) const
{
	FloatImage imFilter(im.width(), im.height(), im.channels());

	int sideW = int((width - 1.0) / 2.0);
	int sideH = int((height - 1.0) / 2.0);
	float sumVal;

	// for every pixel in the image
	for (int x = 0; x < imFilter.width(); x++)
		for (int y = 0; y < imFilter.height(); y++)
			for (int z = 0; z < imFilter.channels(); z++)
			{
				sumVal = 0.0;

				for (int xFilter = 0; xFilter < width; xFilter++)
					for (int yFilter = 0; yFilter < height; yFilter++)
					{

						// sum the image pixel values weighted by the filter
						// TODO: Give a hint in the document to use operator()(xFilter, yFilter)
						sumVal += operator()(xFilter, yFilter)
									* im.smartAccessor(x - xFilter + sideW, y - yFilter + sideH, z, clamp);

					}

				// assign the pixel the value from convolution
				imFilter(x, y, z) = sumVal;
			}

	return imFilter;
}


/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

Filter::Filter(const vector<float> &fData, const int &fWidth, const int &fHeight)
{
	// TODO: check that width*height = length of filterVals and that width and height are odd

	kernel = fData;
	width = fWidth;
	height = fHeight;

}

Filter::Filter(const int &fWidth, const int &fHeight)
{
	width = fWidth;
	height = fHeight;
	kernel = std::vector<float>(width * height, 0);
}

const float &Filter::operator()(int x, int y) const
{
	if (x < 0 || x >= width)
		throw OutOfBoundsException();
	if (y < 0 || y >= height)
		throw OutOfBoundsException();

	return kernel[x + y * width];
}

float &Filter::operator()(int x, int y)
{
	if (x < 0 || x >= width)
		throw OutOfBoundsException();
	if (y < 0 || y >= height)
		throw OutOfBoundsException();

	return kernel[x + y * width];
}
Filter::~Filter() {}