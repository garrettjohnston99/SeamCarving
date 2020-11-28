
#include "filtering.h"

FloatImage minPath(const FloatImage &im) {
    int width = im.width();
    int height = im.height();
    int channels = im.channels();

    FloatImage energy = gradientMagnitude(im);
    energy.write(DATA_DIR "/output/energy.png");
    FloatImage dp(width, height, 1);

    // Fill in bottom row of dp with bottom row of energy
    for (int x = 0; x < width; x++) {
        dp(x, height - 1) = energy(x, height - 1);
    }

    // Now fill in dp bottom-up
    // Each dp(x, y) is equal to energy(x, y) plus the minimum value below (x, y) in dp
    // Considering the three pixels below it - (x - 1, y + 1), (x, y + 1), (x + 1, y + 1)
    for (int y = height - 2; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            if (x == 0) {
                dp(x, y) = energy(x, y) + min(dp(x, y + 1), dp(x + 1, y + 1));
            } else if (x == width - 1) {
                dp(x, y) = energy(x, y) + min(dp(x, y + 1), dp(x - 1, y + 1));
            } else {
                dp(x, y) = energy(x, y) + min(dp(x, y + 1), 
                                              min(dp(x - 1, y + 1), dp(x + 1, y + 1)));
            }
        }
    }

    // Find x-coordinate of minimum value on top row of dp for tracking minimum energy seam
    int minX;
    int topMin = INT_MAX;
    for (int x = 0; x < width; x++) {
        if (dp(x, 0) < topMin) {
            minX = x;
            topMin = dp(x, 0);
        }
    }

    // Now find the minimum seam starting at (minX, 0)
    FloatImage res(im);
    int x = minX;
    for (int c = 0; c < channels; c++) {
        res(x, 0, c) = -1;
    }

    // Find the minimum seam, marking pixels in the seam with -1
    for (int y = 0; y < height - 1; y++) {
        float leftCost = x == 0 ? __FLT_MAX__ : dp(x - 1, y + 1);
        float middleCost = dp(x, y + 1);
        float rightCost = x == width - 1 ? __FLT_MAX__ : dp(x + 1, y + 1);

        float minCost = min(leftCost, min(middleCost, rightCost));
        if (minCost == leftCost) {
            x--;           
        } else if (minCost == rightCost) {
            x++;
        }

        // Mark as part of minimum energy seam
        for (int c = 0; c < channels; c++) {
            res(x, y + 1, c) = -1;
        }
    }
    
    return res;
}


// Assumes that im is an image that has had a seam marked using minPath
// Removes the seam, returning an image with dimensions (im.width() - 1, im.height(), im.channels())
FloatImage removeSeam(const FloatImage &im) {
    int width = im.width() - 1;
    int height = im.height();
    int channels = im.channels();
    
    FloatImage res(width, height, channels);

    // Copy all valid pixels(not -1) over to result
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (im(x, y) != -1.0f) {
                for (int c = 0; c < channels; c++) {
                    res(x, y, c) = im(x, y, c);
                }
            }
        }
    }

    return res;
}


// Assumes that im is an image that has had a seam marked using minPath
// Marks the seam with given rgb color, defaults to yellow
FloatImage markSeam(const FloatImage &im, const float r, const float g, const float b) {
    int width = im.width();
    int height = im.height();

    FloatImage res(im);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (res(x, y) == -1.0f) {
                res(x, y, 0) = r;
                res(x, y, 1) = g;
                res(x, y, 2) = b;
            }
        }
    }

    return res;
}
