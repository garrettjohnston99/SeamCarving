#include "a2.h"
#include <math.h>
#include "utils.h"
#include <assert.h>
#include <iostream>

using namespace std;

/* 2 functions you've already implemented in assignment 1 */

// Change the brightness of the image
// const FloatImage & means a reference to im will get passed to you,
// but the compiler won't let you modify it
FloatImage brightness(const FloatImage &im, float factor)
{
	FloatImage output = FloatImage(im);
	for (int i = 0; i < output.size(); i++)
	{
		output(i) = output(i) * factor;
	}
	return output;
}

// Increases the contrast of an image around a specified midpoint by the value:
// factor
FloatImage contrast(const FloatImage &im, float factor, float midpoint)
{
	FloatImage output = FloatImage(im);
	for (int i = 0; i < output.size(); i++)
	{
		output(i) = factor * (output(i) - midpoint) + midpoint;
	}
	return output;
}

/*New tasks in assignment 2 */

FloatImage changeGamma(const FloatImage &im, float old_gamma, float new_gamma)
{
	// create an appropriately sized output FloatImage
	FloatImage output(im);

	// Take x^(old_gamma) to the power of (new_gamma / old_gamma), which equals
	// x^(old_gamma * (new_gamma / old_gamma)) = x^(new_gamma)
	float gamma_ratio = new_gamma / old_gamma;
	for (int i = 0; i < output.size(); i++)
	{
		output(i) = pow(output(i), gamma_ratio);
	}

	return output;
}

// Change the exposure of the image. This is different than brightness because
// it means you multiply an image's intensity in the linear domain.
FloatImage exposure(const FloatImage &im, float factor)
{
	// create an appropriately sized output FloatImage
	FloatImage output(im);

	// Assume that image is encoded with gamma = 1/2.2
	float gamma = 1 / 2.2;

	// gamma decode the image
	output = changeGamma(output, gamma, 1);

	// modify exposure on the gamma decoded image
	for (int i = 0; i < output.size(); i++)
	{
		output(i) = output(i) * factor;
	}

	// gamma encode the image back again with gamma = 1/2.2
	output = changeGamma(output, 1, gamma);

	return output;
}

// This function should output a grayscale image by performing a weighted
// average across color channels of an input image im. The weights are stored
// in the length 3 vector std::vector<float> & weights. The image returned
// should be a three dimensional (not two dimensional) one channel image.
FloatImage color2gray(const FloatImage &im, const vector<float> &weights)
{
	if ((int)weights.size() != im.channels())
		throw MismatchedDimensionsException();
	const float weights_sum = weights[0] + weights[1] + weights[2];
	FloatImage output = FloatImage(im.width(), im.height(), 1);
	const int im_area = im.width() * im.height();
	// Convert to grayscale by getting the weighted average of the rgb values of
	// each pixel in im. I am assuming that weights[0] = weight for red value,
	// weights[1] = weight for green value, and weights[2] = weight for blue value
	// I am also assuming that the FloatImage has the red pixel value at
	// z_stride = 0, green value at z_stride = 1, and blue value at z_stride = 2.
	for (int i = 0; i < output.size(); i++)
	{
		output(i) = (im(i) * weights[0]) +
								(im(i + im_area) * weights[1]) +
								(im(i + 2 * im_area) * weights[2]) /
										weights_sum;
	}

	return output;
}

// For this function, we want two outputs, a single channel luminance image
// and a three channel chrominance image. Return them in a vector with luminance first
vector<FloatImage> lumiChromi(const FloatImage &im)
{
	vector<FloatImage> output;

	// Create luminance image with default weights as defined in a2.h
	FloatImage luminance_im = color2gray(im);

	// Create  chrominance image
	// there are cases where luminance will be 0 but chrominance will not be 0 due
	// to floating point errors. To account for these instances set chrominance \
	// equal to the original image divided by the sum of luminance and some
	// epsilon value (1e-9 works).
	const float epsilon = 1 * pow(10, -9);
	FloatImage chrominance_im = im / (luminance_im + epsilon);

	// push the luminance and chrominance images onto the vector
	output.push_back(luminance_im);
	output.push_back(chrominance_im);
	return output;
}

// go from a luminance/chrominance images back to an rgb image
FloatImage lumiChromi2rgb(const FloatImage &lumi, const FloatImage &chromi)
{
	FloatImage output(chromi);

	// combine the luminance and chrominance images into an rgb image
	output = chromi * lumi;

	return output;
}

// Modify brightness then contrast
FloatImage brightnessContrastLumi(const FloatImage &im, float brightF, float contrastF, float midpoint)
{
	// Create output image of appropriate size
	FloatImage output(im);

	// Decompose the image into luminance and chrominance
	vector<FloatImage> lc_im = lumiChromi(im);
	FloatImage lumi_im = lc_im[0];
	FloatImage chromi_im = lc_im[1];

	// Modify brightenss of luminance image
	lumi_im = brightness(lumi_im, brightF);

	// Modify the contrast of the now brightened luminance image
	lumi_im = contrast(lumi_im, contrastF);

	// Recombine the modified luminance image with the chrominance image
	output = lumiChromi2rgb(lumi_im, chromi_im);

	return output;
}

FloatImage rgb2yuv(const FloatImage &im)
{
	// Create output image of appropriate size
	FloatImage output(im);
	const int im_area = im.width() * im.height();
	// Change colorspace
	for (int i = 0; i < im_area; i++)
	{
		// Calculate 'Y' in YUV
		output(i) = im(i) * 0.299f + im(i + im_area) * 0.587f + im(i + 2 * im_area) * 0.114f;

		// Calculate 'U' in YUV
		output(i + im_area) = im(i) * -0.147f + im(i + im_area) * -0.289f + im(i + 2 * im_area) * 0.436f;

		// Calculate 'V' in YUV
		output(i + 2 * im_area) = im(i) * 0.615f + im(i + im_area) * -0.515f + im(i + 2 * im_area) * -0.100f;
	}

	return output;
}

FloatImage yuv2rgb(const FloatImage &im)
{
	// Create output image of appropriate size
	FloatImage output(im);
	const int im_area = im.width() * im.height();
	// Change colorspace
	for (int i = 0; i < im_area; i++)
	{
		// Calculate red value
		output(i) = im(i) * 1.0f + im(i + im_area) * 0.0f + im(i + 2 * im_area) * 1.14f;

		// Calculate green value
		output(i + im_area) = im(i) * 1.0f + im(i + im_area) * -0.395f + im(i + 2 * im_area) * -0.581f;

		// Calculate blue value
		output(i + 2 * im_area) = im(i) * 1.0f + im(i + im_area) * 2.032f + im(i + 2 * im_area) * 0.0f;
	}

	return output;
}

FloatImage saturate(const FloatImage &im, float factor)
{
	// Create output image of appropriate size
	FloatImage output(im);
	const int im_area = im.width() * im.height();

	// change output to YUV colorspace
	output = rgb2yuv(output);

	// Multiply U and V values by factor
	for (int i = 0; i < im_area; i++)
	{
		// Scale the U value
		output(i + im_area) = output(i + im_area) * factor;
		// Scale the V value
		output(i + 2 * im_area) = output(i + 2 * im_area) * factor;
	}

	// change output back to rgb colorspace
	output = yuv2rgb(output);

	return output;
}

// Return two images in a C++ vector
vector<FloatImage> spanish(const FloatImage &im)
{
	// Remember to create the output images and the output vector
	vector<FloatImage> output;
	const int im_area = im.height() * im.width();

	// change colorspace to YUV
	FloatImage yuv_im = rgb2yuv(im);

	// Create first image which has constant luminance(Y) = 0.5 and negative U & V
	FloatImage const_lumi_im(yuv_im);
	for (int i = 0; i < im_area; i++)
	{
		const_lumi_im(i) = 0.5f * const_lumi_im(i);
		const_lumi_im(i + im_area) = -1.0f * const_lumi_im(i + im_area);
		const_lumi_im(i + 2 * im_area) = -1.0f * const_lumi_im(i + 2 * im_area);
	}

	// Create second image which is grayscale (U and V = 0)
	FloatImage gray_im(yuv_im);
	for (int i = 0; i < im_area; i++)
	{
		gray_im(i + im_area) = 0;
		gray_im(i + 2 * im_area) = 0;
	}

	// Add a black dot (rectangle) to the center of both images
	vector<int> black_dot_location;
	const int dot_radius = 2;
	black_dot_location.push_back(floor(im.width() / 2));
	black_dot_location.push_back(floor(im.height() / 2));
	for (int i = -dot_radius; i < dot_radius; i++)
	{
		for (int j = -dot_radius; j < dot_radius; j++)
		{
			const_lumi_im(black_dot_location[0] + i, black_dot_location[1] + j) = 0;
			gray_im(black_dot_location[0] + i, black_dot_location[1] + j) = 0;
		}
	}

	// Change colorspace back to rgb
	const_lumi_im = yuv2rgb(const_lumi_im);
	gray_im = yuv2rgb(gray_im);

	// Push the images onto the vector
	output.push_back(const_lumi_im);
	output.push_back(gray_im);

	// Return the vector
	return output;
}

// White balances an image using the gray world assumption
// It will output an image with an average gray value equal to the average
// value of the green channel of the input image.
FloatImage grayworld(const FloatImage &im)
{
	// Your code goes here
	FloatImage output(im);
	const int im_area = im.width() * im.height();

	// Calculate the average value of each color channel of the input image
	float total_red = 0.0f;
	float total_green = 0.0f;
	float total_blue = 0.0f;
	for (int i = 0; i < im_area; i++)
	{
		total_red += im(i);
		total_green += im(i + im_area);
		total_blue += im(i + 2 * im_area);
	}
	float avg_red = total_red / im_area;
	float avg_green = total_green / im_area;
	float avg_blue = total_blue / im_area;

	// White balance every pixel by multiplying each color channel by
	// avg_green / avg_this_color_channel
	for (int i = 0; i < im_area; i++)
	{
		output(i) = output(i) * avg_green / avg_red;
		output(i + 2 * im_area) = output(i + 2 * im_area) * avg_green / avg_blue;
	}

	return output;
}

// Histograms

// Stretches the pixel values of channel c of im so that the minimum value maps to 0,
// and the maximum value maps to 1
void autoLevels(FloatImage &im, int c)
{
	// stretch pixel values of channel c to fill 0..1 range
	// you may find FloatImage::min and FloatImage::max useful
	// Identify min and max value in channel c
	const float c_max = im.max(c);
	const float c_min = im.min(c);
	float im_range_c = c_max - c_min;
	const int im_area = im.width() * im.height();
	for (int i = 0; i < im_area; i++)
	{
		// calculate the new position of this pixel value with the range stretched to 0..1
		float stretched_value = (im(i + c * im_area) - c_min) / im_range_c;
		im(i + c * im_area) = stretched_value;
	}
}

// initialize a histogram with numBins bins from the c-th channel of im
Histogram::Histogram(const FloatImage &im, int c, int numBins) : m_pdf(numBins, 0.0f), m_cdf(numBins, 0.0f)
{
	const int im_area = im.width() * im.height();
	FloatImage im_copy(im);

	// stretch the pixel value range of channel c to 0..1
	autoLevels(im_copy, c);

	// populate m_pdf with histogram values
	for (int i = 0; i < im_area; i++)
	{
		float numBinsInverse = 1.0f / numBins;
		int binIndex = floor(im_copy(i + c * im_area) / numBinsInverse);
		// This fixes the case where the pixel value is 1 so it is mapped to
		// binIndex = numBins which is an invalid index
		binIndex = binIndex == numBins ? binIndex - 1 : binIndex;
		m_pdf[binIndex] += 1.0f;
	}

	// normalize the counts in the histogram so that they sum to 1
	// we know total count is the number of pixels in one channel which is equal
	// to the im_area
	for (int i = 0; i < numBins; i++)
	{
		m_pdf[i] = m_pdf[i] / im_area;
	}

	// Grad/extra credit: populate m_cdf with running sum of m_pdf
	float runningSum = 0.0f;
	for (int i = 0; i < numBins; i++)
	{
		runningSum += m_pdf[i];
		m_cdf[i] = runningSum;
	}
}

// return the histogram bin that value falls within
int Histogram::inverseCDF(double value) const
{
	return 0; // CHANGEME
}

// Produce a numBins() x 100 x 3 image containing a visualization of the
// red, green and blue histogram pdfs
FloatImage visualizeRGBHistogram(const Histogram &histR,
																 const Histogram &histG,
																 const Histogram &histB)
{
	assert(histR.numBins() == histG.numBins() && histR.numBins() == histB.numBins());
	// create an image of appropriate size
	FloatImage im(histR.numBins(), 100, 3);
	const int im_area = im.width() * im.height();
	const int numBins = histR.numBins();

	Histogram currHist;

	// Calculate the max bin value across all the channels
	float maxBinValue = 0.0f;
	for (int c = 0; c < 3; c++)
	{
		switch (c)
		{
		case 0:
			currHist = histR;
			break;
		case 1:
			currHist = histG;
			break;
		case 2:
			currHist = histB;
			break;
		}
		for (int i = 0; i < im.width(); i++)
		{
			maxBinValue = maxBinValue < currHist.pdf(i) ? currHist.pdf(i) : maxBinValue;
		}
	}

	// populate im with RGB histograms
	// Color the hist bars one hist at a time.
	for (int c = 0; c < 3; c++)
	{
		switch (c)
		{
		case 0:
			currHist = histR;
			break;
		case 1:
			currHist = histG;
			break;
		case 2:
			currHist = histB;
			break;
		}

		// for each hist, go column by column (bin by bin)
		for (int i = 0; i < im.width(); i++)
		{
			// for each column (bin), multiply the bin count by 100 so that the total
			// bin count sums to 100 rather than 1. Color the number of pixels to
			// equal the new bin count the same color as the current hist.
			float binCount = currHist.pdf(i) * 100.0f / maxBinValue;
			for (int j = im.height() - 1; j > (im.height() - 1) - binCount; j--)
			{
				im(i + im.width() * j + c * im_area) = 1;
			}
		}
	}

	return im;
}

// Return a FloatImage which is the result of applying histogram equalization to im
FloatImage equalizeRGBHistograms(const FloatImage &im)
{
	int numLevels = 256;

	// create an image of appropriate size
	FloatImage output = im;

	// apply histogram equalization to each channel

	return output;
}

// Return a FloatImage which is the result of transfering the histogram of F2 onto the image data in F1
FloatImage matchRGBHistograms(const FloatImage &F1, const FloatImage &F2)
{
	int numBins = 256;

	FloatImage output = F1;

	// perform histogram matching

	return output;
}
