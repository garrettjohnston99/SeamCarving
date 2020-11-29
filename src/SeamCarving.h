/* 
 * Function prototypes for SeamCarving.cpp
 * 
 * Garrett Johnston & Thanh Nguyen, CS73 20F
 */

#include "floatimage.h"
#include "exceptions.h"

/* 
 * minPathVert: takes a FloatImage im, returning a FloatImage with a vertical seam marked with -1s
 * Uses gradientMagnitude and dynamic programming to find the minimum "cost" from top-bottom of the image
 * The seam varies horizontally by at most one pixel from the pixel above/below
 * To remove the seam, see removeSeam. To mark the seam with a color, see markSeam
 */
FloatImage minPathVert(const FloatImage &im);

/* 
 * minPathVert: takes a FloatImage im, returning a FloatImage with a horizontal seam marked with -1s
 * Uses gradientMagnitude and dynamic programming to find the minimum "cost" from left-right of the image
 * The seam varies vertically by at most one pixel from the pixel to the left/right
 * To remove the seam, see removeSeam. To mark the seam with a color, see markSeam
 */
FloatImage minPathHorizontal(const FloatImage &im);

/* 
 * removeSeam: takes a FloatImage im and bool vertSeam, returning a FloatImage with the seam removed
 * im is assumed to have been created by minPathVert or minPathHorizontal, and vertSeam defaults to true
 * Iterates over each pixel in the image and skips pixels marked as part of the seam(-1)
 */
FloatImage removeSeam(const FloatImage &im, bool vertSeam = true);

/*
 * markSeam: takes a FloatImage im and rgb values, returning a FloatImage the seam marked with the given rgb
 * im is assumed to have been created by minPathVert or minPathHorizontal, and rgb defaults to yellow
 * Iterates over each pixel in the image, recoloring pixels marked as part of the seam(-1)
 */
FloatImage markSeam(const FloatImage &im, const float r = 1.0f, const float g = 1.0f, const float b = 0.0f);

/*
 * removeNSeams: takes a FloatImage im, integer in, and bools verticalSeam and writeIntermediates,
 * returning a FloatImage with size reduced by n on the desired axis
 * verticalSeam defaults to true, writeIntermediates defaults to false
 * Throws NegativeDimensionException if n is greater than the number of pixels on the relevant axis
 * If writeIntermediates is true, saves images produced by markSeam on each iteration
 */
FloatImage removeNSeams(const FloatImage &im, int n, bool verticalSeam = true, bool writeIntermediates = false);

/*
 * retargetImage: takes a FloatImage im, integer newWidth, integer newHeight, and boolean writeIntermediates, and
 * returns a FloatImage with dimensions newWidth x newHeight which is the retargeted image of im using the seam carving 
 * algorithm.
 * Throws NegativeDimensionException if width or height are negative.
 * If writeIntermediates is true, saves images produced by markSeam on each iteration.
 */
FloatImage retargetImage(const FloatImage &im, int newWidth, int newHeight, bool writeIntermediates = false);
