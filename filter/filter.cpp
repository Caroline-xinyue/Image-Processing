#include <iostream>
#include "kernel.h"
#include "filter.h"

using namespace cv;

int readImage(string);
void showImage(string, Mat&);
void showImages();
void writeImages();
void createMat(Mat&);


Mat src, mistery, makeup, sexy, blurry, blonde, blondeBlurry, paint, blondeMakeup;
int numColors = 0;


int main (int argc, char** argv) {
  if(argc == 1) {
    std::cout << "Please run the program with your input file name separated by a white space" << std::endl;
    return -1;
  }
  int invalid = readImage(argv[1]);
  if(invalid) {std::cout << "Error loading image \n"; return -1;}
  if(numColors != 3) {
    std::cout << "Please upload a 3-channel color image \n";
    return -1;
  }
  
  // prepare for Mat
  createMat(makeup);
  createMat(sexy);
  createMat(blurry);
  createMat(blonde);
  createMat(blondeBlurry);
  createMat(paint);
  createMat(blondeMakeup);
  createMat(mistery);
  
  // Apply filters
  
  // makeup
  kern3_1.filter(src, makeup);

  // sexy
  kern3_2.filter(src, sexy);

  // blurry
  kern3_3.filter(src, blurry);

  // blonde
  kern2.filter(src, blonde);

  // blondeBlur
  kern2.filter(src, blondeBlurry);
  kern3_3.filter(blondeBlurry, blondeBlurry);

  // painting-like blondeSexy
  kern2.filter(src, paint);
  kern3_2.filter(paint, paint);

  // blondeMakeup
  kern2.filter(src, blondeMakeup);
  kern3_1.filter(blondeMakeup, blondeMakeup);

  // mistery
  kern5.filter(src, mistery);

  showImages();
  writeImages();
  
  waitKey(0);
  return 0;
}

int readImage(string name)
{
  src = imread(name);
  if(!src.data)
    {
      std::cerr << "Error loading image " << name << std::endl;
      return -1;
    }
  numColors = src.channels();
  return 0;
}

void writeImages()
{
  imwrite("src.png", src);
  imwrite("mistery.png", mistery);
  imwrite("makeup.png", makeup);
  imwrite("sexy.png", sexy);
  imwrite("blurry.png", blurry);
  imwrite("blonde.png", blonde);
  imwrite("blondeBlurry.png", blondeBlurry);
  imwrite("paint.png", paint);
  imwrite("blondeMakeup.png", blondeMakeup);
}


// Create Windows and show image
void showImage(string name, Mat& mat)
{
  namedWindow(name, WINDOW_NORMAL);
  imshow(name, mat);
}

void showImages()
{
  showImage("src", src);
  showImage("mistery", mistery);
  showImage("makeup", makeup);
  showImage("sexy", sexy);
  showImage("blurry", blurry);
  showImage("blonde", blonde);
  showImage("blondeBlurry", blondeBlurry);
  showImage("paint", paint);
  showImage("blondeMakeup", blondeMakeup);
}

void createMat(Mat& mat)
{
  mat = src.clone();
}
