#include "SeamCarving.h"
#include "filtering.h"
#include "utils.h"

using namespace std;

void testMinPathVert()
{
  FloatImage test(DATA_DIR "/input/dali.jpg");

  FloatImage out = minPathVert(test);
  out.write(DATA_DIR "/output/daliOutVert.png");
}

void testMinPathHorizontal()
{
  FloatImage test(DATA_DIR "/input/dali.jpg");

  FloatImage out = minPathHorizontal(test);
  out.write(DATA_DIR "/output/daliOutHorizontal.png");
}

void testMarkPath()
{
  FloatImage test(DATA_DIR "/input/dali.jpg");

  FloatImage marked = minPathVert(test);
  FloatImage colored = markSeam(marked);
  colored.write(DATA_DIR "/output/daliColoredSeam.png");
}

void testRemoveNVert()
{
  FloatImage dali(DATA_DIR "/input/dali.jpg");

  FloatImage res = removeNSeams(dali, 50, true, true);
  res.write(DATA_DIR "/output/dali50.png");
}

void testRemoveNHorizontal()
{
  FloatImage dali(DATA_DIR "/input/dali.jpg");

  FloatImage res = removeNSeams(dali, 50, false, true);
  res.write(DATA_DIR "/output/dali50.png");
}

int main()
{
  //testMinPathHorizontal();
  //testMinPathVert();
  //testMarkPath();
  //testRemoveNVert();
  //testRemoveNHorizontal();
}