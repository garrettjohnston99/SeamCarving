#include "SeamCarving.h"
#include "filtering.h"
#include "utils.h"

using namespace std;

void testMinPath() {
    FloatImage test(DATA_DIR "/input/dali.jpg");

    FloatImage out = minPath(test);
    out.write(DATA_DIR "/output/daliOut.png");
}

void testMarkPath() {
    FloatImage test(DATA_DIR "/input/dali.jpg");

    FloatImage marked = minPath(test);
    FloatImage colored = markSeam(marked);
    colored.write(DATA_DIR "/output/daliColoredSeam.png");
}

int main() {
  testMarkPath();

}