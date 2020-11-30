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

void forwardEnergyExamples()
{
  FloatImage im("data/input/frances-gunn-5k03gTCaEW0-unsplash.jpg");

  FloatImage res1 = removeNSeams(im, 70, true, false, true);
  res1.write(DATA_DIR "/output/exampleForwardEnergy.png");

  FloatImage res2 = removeNSeams(im, 70, true, false, false);
  res2.write(DATA_DIR "/output/exampleNoForwardEnergy.png");
}

void starryNightEnergy()
{
  FloatImage im("data/input/starryNight.jpg");

  FloatImage energy = gradientMagnitude(im);
  energy.write(DATA_DIR "/output/starryNightEnergy.png");
}
void trimIdiots()
{
  FloatImage idiots(DATA_DIR "/input/dali.JPG");
  FloatImage trimmed = removeNSeams(idiots, 1, true, true, false);
  trimmed.write(DATA_DIR "/output/daliTrimmed100.png");
}

int main()
{
  //testMinPathHorizontal();
  //testMinPathVert();
  //testRemoveNVert();
  //testRemoveNHorizontal();
  //testRetargetImage();
  //forwardEnergyExamples();
  starryNightEnergy();
  trimIdiots();
}
