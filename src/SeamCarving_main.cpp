/* 
 * Test driver for functions in SeamCarving.cpp
 *
 * Garrett Johnston & Thanh Nguyen, CS73 20F
 */

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
  marked.write(DATA_DIR "/output/daliColoredSeam.png");
}

void testRemoveNVert()
{
  FloatImage dali(DATA_DIR "/input/dali.jpg");

  FloatImage res1 = removeNSeams(dali, 50, true, true, true);
  res1.write(DATA_DIR "/output/dali50VertForwardEnergy.png");

  FloatImage res2 = removeNSeams(dali, 50, true, true, false);
  res2.write(DATA_DIR "/output/dali50VertNoForwardEnergy.png");

  FloatImage meadow(DATA_DIR "/input/greenMeadow.jpg");

  FloatImage res3 = removeNSeams(meadow, 50, true, true, true);
  res3.write(DATA_DIR "/output/greenMeadow50VertForwardEnergy.png");

  FloatImage res4 = removeNSeams(meadow, 50, true, true, false);
  res4.write(DATA_DIR "/output/greenMeadow50VertNoForwardEnergy.png");
}

void testRemoveNHorizontal()
{
  FloatImage dali(DATA_DIR "/input/dali.jpg");

  FloatImage res1 = removeNSeams(dali, 50, false, true, true);
  res1.write(DATA_DIR "/output/dali50HoriForwardEnergy.png");
  FloatImage res2 = removeNSeams(dali, 50, false, true, false);
  res2.write(DATA_DIR "/output/dali50HoriNoForwardEnergy.png");
}

void testRetargetImage()
{
  FloatImage dali(DATA_DIR "/input/dali.jpg");

  FloatImage res = retargetImage(dali, 200, 200, false);
  res.write(DATA_DIR "/output/daliRetargeted.png");
}

int main()
{
  //testMinPathHorizontal();
  //testMinPathVert();
  testRemoveNVert();
  testRemoveNHorizontal();
  //testRetargetImage();
}
