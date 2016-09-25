#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream> // std::cout std::cin
#include <algorithm> // std::min

using namespace cv;

void readImage(char* path1, char* path2);
void showImage(string, Mat&);
void writeImage(std::string path);
void slice(int, int);
void miniature(int, int, int);
void combine(int, int, int);

Mat img1, img2, dst;
vector<Mat> pieces1, pieces2;
int size = 0;
int numGrid = 8;

int main(int argc, char** argv)
{
  if(argc != 4) {
    std::cout << "Please run the program with your two input image file names and the desired output file name, separated by a white space" << std::endl;
    return -1;
  }
  readImage(argv[1], argv[2]);
  int width = dst.cols/numGrid;
  int height = dst.rows/numGrid;


  char choice = 'm';
  std::cout << "Please input your choice of fancyBoard: m for miniature or c for combine \n";
  std::cin >> choice;
  
  if(choice == 'm') {
    miniature(numGrid, width, height);
  } else if(choice == 'c') {
    combine(numGrid, width, height);
  }
  
  showImage("img1", img1);
  showImage("img2", img2);
  showImage(argv[3], dst);

  writeImage(argv[3]);
  
  waitKey(0);
  return 0;
}

/*
  Create the fancyBoard by putting the miniature of the two input image files side by side into a square board.
  References:
  http://docs.opencv.org/2.4/modules/imgproc/doc/geometric_transformations.html#void%20resize%28InputArray%20src,%20OutputArray%20dst,%20Size%20dsize,%20double%20fx,%20double%20fy,%20int%20interpolation%29
  http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat-copyto
  http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat
 */
void miniature(int numGrid, int width, int height)
{
  Mat part1, part2;

  // miniature
  resize(img1, part1, Size(width, height));
  resize(img2, part2, Size(width, height));
  
  int sum = 0;
  for(int i = 0; i < numGrid; i++) {
    for(int j = 0; j < numGrid; j++) {
      sum = i + j;
      if(sum % 2 == 0) {
	part1.copyTo(dst(Rect(i * width, j * height, width, height))); // openCV ROI
      } else {
	part2.copyTo(dst(Rect(j * width, i * height, width, height))); // openCV ROI
      }
    }
  }
}

/*
  Create the fancyBoard by splitting each of the two input image files and combine them side by side into a square board.

  References:
  http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat-copyto
  http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat
 */
void combine(int numGrid, int width, int height)
{ 
  // slice
  slice(width, height);
  slice(width, height);

  int sum = 0;
  int m = 0;
  int n = 0;
  for(int i = 0; i < numGrid; i++) {
    for(int j = 0; j < numGrid; j++) {
      sum = i + j;
      if(sum % 2 == 0) {
	pieces1[m++].copyTo(dst(Rect(j * width, i * height, width, height)));
      } else {
	pieces2[n++].copyTo(dst(Rect(j * width, i * height, width, height)));
      }
    }
  }
}

/*
  Slice the image into small pieces and store them into a vector of Mat.

  References:
  http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#Mat::Mat%28const%20Mat&%20m,%20const%20Rect&%20roi%29
  http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html
 */
void slice(int width, int height)
{
  int sum = 0;
  for(int i = 0; i < numGrid; i++) {
    for(int j = 0; j < numGrid; j++) {
      Rect rect = Rect(j * width, i * height, width, height);
      sum = i + j;
      if(sum % 2 == 0) {
	pieces1.push_back(Mat(img1, rect));
      } else {
	pieces2.push_back(Mat(img2, rect));
      }
    }
  }
}

void readImage(char* path1, char* path2)
{
  img1 = imread(path1);
  img2 = imread(path2);
  
  int width1 = img1.size().width;
  int height1 = img1.size().height;
  int width2 = img2.size().width;
  int height2 = img2.size().height;
  size = std::min(std::min(width1, height1), std::min(width2, height2));
  dst = Mat(size, size, CV_8UC3);
}

void showImage(string name, Mat& mat)
{
  namedWindow(name, WINDOW_NORMAL);
  imshow(name, mat);
}

void writeImage(std::string path)
{
  // Write the image
  if(!dst.data)
    {
      std::cout << "Error writing image \n";
    }
  imwrite(path, dst);
}
