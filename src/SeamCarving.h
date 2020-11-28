#include "floatimage.h"
#include "exceptions.h"

FloatImage minPath(const FloatImage &im);

FloatImage removeSeam(const FloatImage &im);

FloatImage markSeam(const FloatImage &im, const float r=1.0f, const float g=1.0f, const float b=0.0f);

FloatImage removeNSeams(const FloatImage &im, int n, bool writeIntermediates=false);
