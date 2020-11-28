#ifndef __FILTERING_H
#define __FILTERING_H

// Assignment 4
// Filtering and convolution

#include "floatimage.h"
#include <iostream>
#include <math.h>

using namespace std;


class Filter
{
public:
    std::vector<float> kernel;
    int width;
    int height;
    
    // function to convolve your filter with an image
    FloatImage Convolve(const FloatImage &im, bool clamp=true) const;
    
    // Accessors of the filter values
    const float & operator()(int x, int y) const;
    float & operator()(int x, int y);
    
    //Constructor
    Filter(const vector<float> &fData, const int &fWidth, const int &fHeight);
    Filter(const int &fWidth, const int &fHeight); // kernel with all zero

    // Destructor. Because there is no explicit memory management here, this doesn't do anything
    ~Filter();
    
// The following are functions and variables that are not accessible from outside the class
private:
    
};

// Gradient Filter
FloatImage gradientMagnitude(const FloatImage &im, bool clamp=true);

 
#endif
