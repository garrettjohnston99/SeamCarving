/* 
 * Function prototypes for SeamCarving.cpp
 * 
 * Garrett Johnston & Thanh Nguyen, CS73 20F
 */

#include "floatimage.h"
#include "exceptions.h"

/*
 * pixelDiff: takes a FloatImage im, and 2 coordinates pairs, x1, y1, x2, y2
 * and returns the squared difference between the channel values of the pixels at the designated coordinates.
 */
float pixelDiff(const FloatImage &im, int x1, int y1, int x2, int y2);

/* 
 * minPathVert: takes a FloatImage im, returning a FloatImage with a vertical seam with G channel marked -1
 * When displayed, the returned FloatImage highlights the seam in yellow.
 * Uses gradientMagnitude and dynamic programming to find the minimum "cost" from top-bottom of the image
 * The seam varies horizontally by at most one pixel from the pixel above/below
 * To remove the seam, see removeSeam. 
 */
FloatImage minPathVert(const FloatImage &im, bool applyForwardEnergy = true);

/* 
 * minPathVert: takes a FloatImage im, returning a FloatImage with a horizontal seam with G channel marked -1
 * When displayed, the returned FloatImage highlights the seam in yellow.
 * Uses gradientMagnitude and dynamic programming to find the minimum "cost" from left-right of the image
 * The seam varies vertically by at most one pixel from the pixel to the left/right
 * To remove the seam, see removeSeam.
 */
FloatImage minPathHorizontal(const FloatImage &im, bool applyForwardEnergy = true);

/* 
 * removeSeam: takes a FloatImage im and bool vertSeam, returning a FloatImage with the seam removed
 * im is assumed to have been created by minPathVert or minPathHorizontal, and vertSeam defaults to true
 * Iterates over each pixel in the image and skips pixels marked as part of the seam(-1)
 */
FloatImage removeSeam(const FloatImage &im, bool vertSeam = true);

/*
 * removeNSeams: takes a FloatImage im, integer in, and bools verticalSeam and writeIntermediates,
 * returning a FloatImage with size reduced by n on the desired axis
 * verticalSeam defaults to true, writeIntermediates defaults to false
 * Throws NegativeDimensionException if n is greater than the number of pixels on the relevant axis
 * If writeIntermediates is true, saves images produced by markSeam on each iteration
 */
FloatImage removeNSeams(const FloatImage &im, int n, bool verticalSeam = true, bool writeIntermediates = false, bool applyForwardEnergy = true);

/*
 * retargetImage: takes a FloatImage im, integer newWidth, integer newHeight, and boolean writeIntermediates, and
 * returns a FloatImage with dimensions newWidth x newHeight which is the retargeted image of im using the seam carving 
 * algorithm.
 * Throws NegativeDimensionException if width or height are negative.
 * If writeIntermediates is true, saves images produced by markSeam on each iteration.
 */
FloatImage retargetImage(const FloatImage &im, int newWidth, int newHeight, bool writeIntermediates = false, bool applyForwardEnergy = true);
