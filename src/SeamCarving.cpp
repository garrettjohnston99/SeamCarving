/* 
 * Functions for seam carving(content-aware image resizing)
 * See SeamCarving.h for documentation
 * 
 * Garrett Johnston & Thanh Nguyen, CS73 20F
 */


#include "filtering.h"
#include "SeamCarving.h"
#include <sstream>

FloatImage minPathVert(const FloatImage &im)
{
    int width = im.width();
    int height = im.height();

    FloatImage energy = gradientMagnitude(im);
    energy.write(DATA_DIR "/output/energy.png");
    FloatImage dp(width, height, 1);

    // Fill in bottom row of dp with bottom row of energy
    for (int x = 0; x < width; x++)
    {
        dp(x, height - 1) = energy(x, height - 1);
    }

    // Now fill in dp bottom-up
    // Each dp(x, y) is equal to energy(x, y) plus the minimum value below (x, y) in dp
    // Considering the three pixels below it - (x - 1, y + 1), (x, y + 1), (x + 1, y + 1)
    for (int y = height - 2; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            if (x == 0)
            {
                dp(x, y) = energy(x, y) + min(dp(x, y + 1), dp(x + 1, y + 1));
            }
            else if (x == width - 1)
            {
                dp(x, y) = energy(x, y) + min(dp(x, y + 1), dp(x - 1, y + 1));
            }
            else
            {
                dp(x, y) = energy(x, y) + min(dp(x, y + 1),
                                              min(dp(x - 1, y + 1), dp(x + 1, y + 1)));
            }
        }
    }

    // Find x-coordinate of minimum value on top row of dp for tracking minimum energy seam
    int minX;
    int topMin = INT_MAX;
    for (int x = 0; x < width; x++)
    {
        if (dp(x, 0) < topMin)
        {
            minX = x;
            topMin = dp(x, 0);
        }
    }

    // Now find the minimum seam starting at (minX, 0)
    FloatImage res(im);
    int x = minX;
    res(x, 0, 0) = 1.0f;
    res(x, 0, 1) = 1.0f;
    res(x, 0, 2) = -1.0f;

    // Find the minimum seam, marking pixels in the seam with -1
    for (int y = 0; y < height - 1; y++)
    {
        float leftCost = x == 0 ? __FLT_MAX__ : dp(x - 1, y + 1);
        float middleCost = dp(x, y + 1);
        float rightCost = x == width - 1 ? __FLT_MAX__ : dp(x + 1, y + 1);

        float minCost = min(leftCost, min(middleCost, rightCost));
        if (minCost == leftCost)
        {
            x--;
        }
        else if (minCost == rightCost)
        {
            x++;
        }

        // Mark as part of minimum energy seam. 
        res(x, y + 1, 0) = 1.0f;
        res(x, y + 1, 1) = 1.0f;
        res(x, y + 1, 2) = -1.0f;
    }

    return res;
}

FloatImage minPathHorizontal(const FloatImage &im)
{
    int width = im.width();
    int height = im.height();

    FloatImage energy = gradientMagnitude(im);
    //energy.write(DATA_DIR "/output/energy.png");
    FloatImage dp(width, height, 1);

    // Fill in right-most column of dp with right-most column of energy
    for (int y = 0; y < height; y++)
    {
        dp(width - 1, y) = energy(width - 1, y);
    }

    // Now fill in dp column by columm from right to left
    // Each dp(x, y) is equal to energy(x, y) plus the minimum value to the right (x, y) in dp
    // Considering the three pixels to the right: (x + 1, y - 1), (x + 1, y), (x + 1, y + 1)
    for (int x = width - 2; x >= 0; x--)
    {
        for (int y = 0; y < height; y++)
        {
            if (y == 0)
            {
                dp(x, y) = energy(x, y) + min(dp(x + 1, y), dp(x + 1, y + 1));
            }
            else if (y == height - 1)
            {
                dp(x, y) = energy(x, y) + min(dp(x + 1, y), dp(x + 1, y - 1));
            }
            else
            {
                dp(x, y) = energy(x, y) + min(dp(x + 1, y - 1),
                                              min(dp(x + 1, y), dp(x + 1, y + 1)));
            }
        }
    }

    // Find y-coordinate of minimum value on left-most column of dp for tracking minimum energy seam
    int minY;
    int topMin = INT_MAX;
    for (int y = 0; y < height; y++)
    {
        if (dp(0, y) < topMin)
        {
            minY = y;
            topMin = dp(y, 0);
        }
    }

    // Now find the minimum seam starting at (0, minY)
    FloatImage res(im);
    int y = minY;
    res(0, y, 0) = 1.0f;
    res(0, y, 1) = 1.0f;
    res(0, y, 2) = -1.0f;

    // Find the minimum seam, marking pixels in the seam with -1
    for (int x = 0; x < width - 1; x++)
    {
        float upCost = y == 0 ? __FLT_MAX__ : dp(x + 1, y - 1);
        float middleCost = dp(x + 1, y);
        float downCost = y == height - 1 ? __FLT_MAX__ : dp(x + 1, y + 1);

        float minCost = min(upCost, min(middleCost, downCost));
        if (minCost == upCost)
        {
            y--;
        }
        else if (minCost == downCost)
        {
            y++;
        }

        // Mark as part of minimum energy seam
        res(x + 1, y, 0) = 1.0f;
        res(x + 1, y, 1) = 1.0f;
        res(x + 1, y, 2) = -1.0f;
    }

    return res;
}

// Assumes that im is an image that has had a seam marked using minPath
// Removes the seam, returning an image with dimensions (im.width() - 1, im.height(), im.channels())
// or (im.width(), im.height() - 1, im.channels()) depending on if the seam is vert or horizontal.
FloatImage removeSeam(const FloatImage &im, bool vertSeam)
{
    int width = vertSeam ? im.width() - 1 : im.width();
    int height = !vertSeam ? im.height() - 1 : im.height();
    int channels = im.channels();

    FloatImage res(width, height, channels);

    // Copy all valid pixels(not -1) over to result
    if (vertSeam)
    {
        for (int y = 0; y < height; y++)
        {
            for (int imX = 0, resX = 0; imX < width; imX++)
            {
                if (im(imX, y, 2) != -1.0f)
                {
                    for (int c = 0; c < channels; c++)
                    {
                        res(resX, y, c) = im(imX, y, c);
                    }
                    resX++;
                }
            }
        }
    }
    else
    {
        for (int x = 0; x < width; x++)
        {
            for (int imY = 0, resY = 0; imY < height; imY++)
            {
                if (im(x, imY, 2) != -1.0f)
                {
                    for (int c = 0; c < channels; c++)
                    {
                        res(x, resY, c) = im(x, imY, c);
                    }
                    resY++;
                }
            }
        }
    }

    return res;
}


// Assumes that im is an image that has had a seam marked using minPath
// Marks the seam with given rgb color, defaults to yellow
FloatImage removeNSeams(const FloatImage &im, int n, bool verticalSeam, bool writeIntermediates)
{
    if ((verticalSeam && im.width() < n) || (!verticalSeam && im.height() < n))
        throw NegativeDimensionException();

    FloatImage curr(im);
    for (int i = 0; i < n; i++)
    {
        FloatImage path = verticalSeam ? minPathVert(curr) : minPathHorizontal(curr);

        if (writeIntermediates)
        {
            ostringstream ss;
            ss << DATA_DIR "/output/" << i + 1 << ".png";
            string filename = ss.str();
            path.write(filename);
        }

        curr = removeSeam(path, verticalSeam);
    }

    return curr;
}
