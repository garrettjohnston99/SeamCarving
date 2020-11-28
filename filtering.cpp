// filtering.cpp
// Assignment 4

#include "filtering.h"
#include "a2.h"
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
    sobelX(0,0) = -1.0; sobelX(1,0) = 0.0; sobelX(2,0) = 1.0;
    sobelX(0,1) = -2.0; sobelX(1,1) = 0.0; sobelX(2,1) = 2.0;
    sobelX(0,2) = -1.0; sobelX(1,2) = 0.0; sobelX(2,2) = 1.0;
	FloatImage horizontalGradient = sobelX.Convolve(im, clamp);

	// sobel filtering in y direction
	Filter sobelY(3, 3);
	sobelY(0,0) = -1.0; sobelY(1,0) = -2.0; sobelY(2,0) = -1.0;
    sobelY(0,1) = 0.0; sobelY(1,1) = 0.0; sobelY(2,1) = 0.0;
    sobelY(0,2) = 1.0; sobelY(1,2) = 2.0; sobelY(2,2) = 1.0;
	FloatImage verticalGradient = sobelY.Convolve(im, clamp);

	
	// compute squared magnitude
	float h, v; // squares of pixel values in corresponding component(horizontal/vertical)
	FloatImage output(im.width(), im.height(), im.channels());
	for (int x = 0; x < im.width(); x++) {
		for (int y = 0; y < im.height(); y++) {
			for (int channel = 0; channel < im.channels(); channel++) {
				h = pow(horizontalGradient(x, y, channel), 2.0);
				v = pow(verticalGradient(x, y, channel), 2.0);
				output(x, y, channel) = pow(h + v, 0.5);
			}
		}
	}

	return output;
}



/**************************************************************
 //                 FILTER CLASS FUNCTIONS                  //
 *************************************************************/


// write a convolution function for the filter class
FloatImage Filter::Convolve(const FloatImage &im, bool clamp) const
{
	FloatImage imFilter(im.width(), im.height(), im.channels());

	// implement convolution
	int xFilt, yFilt; // coordinates of filter over image to access image pixel value
	for (int x = 0; x < im.width(); x++) {
		for (int y = 0; y < im.height(); y++) {
			for (int channel = 0; channel < im.channels(); channel++) {
				// Each (x, y) in kernel
				for (int kx = 0; kx < width; kx++) {
					for (int ky = 0; ky < height; ky++) {
						xFilt = x + kx - width / 2;
						yFilt = y + ky - height / 2;
						imFilter(x, y, channel) += im.smartAccessor(xFilt, yFilt, channel) * 
												   operator()(width - 1 - kx, height - 1 - ky); // Flipped kernel
					}
				}
			}
		}
	}

	return imFilter;
}


/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

// Create an image of 0's with a value of 1 in the middle. This function
// can be used to test that you have properly set the kernel values in your
// Filter object. Make sure to set k to be larger than the size of your kernel
FloatImage impulseImg(const int &k)
{
	// initlize a kxkx1 image of all 0's
	FloatImage impulse(k, k, 1);

	// set the center pixel to have intensity 1
	int center = floor(k / 2);
	impulse(center, center, 0) = 1;

	return impulse;
}

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
