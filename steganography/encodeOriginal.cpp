#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <bitset>

using namespace cv;

int readImage(std::string);
void writeImage(std::string);
void showImage(string, Mat&);
void showImages();
std::bitset<8> decTobin(uchar);
void encode(uchar* text);

Mat src, dst;
int numRows = 0;
int numColumns = 0;
int numColors = 0;
uchar* data;
uchar secret;
int msglen = 0;

int main( int argc, char** argv)
{
  int invalid = readImage("beyisbae.png");
  if(invalid) {std::cout << "Error loading image \n"; return -1;}
  if(numColors != 3) {
    std::cout << "Please upload a 3-channel color image \n";
    return -1;
  }
  
  int numPixels = numColumns * numRows;
  uchar text[numPixels];
  
  // Read in secret text
  
  while(std::cin >> secret)
    {
      text[msglen++] = secret;
      if(msglen >= (numPixels - 3)/5){
	std::cout << "Sorry, your message is too long to encode \n";
	return -1;
      }
    }

  encode(text);
  
  showImages();
  writeImage("secretMessage.png");
  waitKey(0);
  return 0;
}

int readImage(std::string path)
{
  // Read in the image
  src = imread(path,CV_LOAD_IMAGE_COLOR);

  
  if(!src.data)
    {
      std::cerr << "Error loading image \n";
      return -1;
    }
  dst = src.clone();
  data = dst.data;
  numRows = dst.rows;
  numColumns = dst.cols;
  numColors = dst.channels();
  return 0;
}

void writeImage(std::string name)
{
  // Write the image
  if(!dst.data)
    {
      std::cout << "Error writing image \n";
    }
  imwrite(name, dst);
}

void showImage(string name, Mat& mat)
{
  namedWindow(name, WINDOW_NORMAL);
  imshow(name, mat);
}

void showImages()
{
  showImage("src", src);
  showImage("dst", dst);
}

/*
  Change the decimal unsigned int number to its corresponding 8 bit binary number
  References: http://www.cplusplus.com/reference/bitset/bitset/
 */
std::bitset<8> decTobin(uchar text)
{
  return std::bitset<8>(text);
}

/*
  Text encode algorithm: First convert the 3 color channel values of each pixel to 8 bit binary. Then we first convert the msglen to 21 bit binary and insert each of the bits into the B, G, R channels of 9 pixels with the color channels of the last 2 pixel unmodified. Finally, convert each text character first to 8 bit binary and insert each of the binary bits to the last bit of the B, G, R channels of three pixels, with the R channel of the last pixel unmodified. Because we only modify the last bit of each color channel, the changes in each pixel is negligible for human eyes.

  References:
  http://docs.opencv.org/2.4/doc/user_guide/ug_mat.html
  http://www.cplusplus.com/reference/bitset/bitset/to_ulong/
 */

void encode(uchar* text)
{
  std::bitset<8> b,g,r,m;
  int n = 0;
  std::bitset<21> l = std::bitset<21>(msglen); // 2^21 = 2097152 > 1200*900 = 1080000
  int k = 20;
  int count = 0;
  int i; int j;
  for(i = 0; i < numColumns; i++) {
    for(j = 0; j < numRows; j++) {
      if(count > msglen) {
	return;
      }
      Vec3b data = dst.at<Vec3b>(i, j);
      b = decTobin(data[0]);
      g = decTobin(data[1]);
      r = decTobin(data[2]);

      if(count == 0) {
	b[0] = l[k--];
	b[1] = l[k--];
	b[2] = l[k--];
	if(k == -1) {
	  m = decTobin(text[n++]);
	  k = 7;
	  count++;
	  dst.at<Vec3b>(i, j)[0] = (uchar)b.to_ulong();
	  continue;
	}
      } else {
	b[0] = m[k--];
      }

      dst.at<Vec3b>(i, j)[0] = (uchar)b.to_ulong();
      
      if(count == 0) {
	g[0] = l[k--];
	g[1] = l[k--];
	g[2] = l[k--];
      } else {
	g[0] = m[k--];
	if(k == -1) {
	  m = decTobin(text[n++]);
	  k = 7;
	  count++;
	  dst.at<Vec3b>(i, j)[1] = (uchar)g.to_ulong();
	  continue;
	}
      }
 
      dst.at<Vec3b>(i, j)[1] = (uchar)g.to_ulong();
 
      if(count == 0) {
	r[0] = l[k--];
	r[1] = l[k--];
	r[2] = l[k--];
      } else {
	r[0] = m[k--];
      }
      dst.at<Vec3b>(i, j)[2] = (uchar)r.to_ulong();
    }
  }
}
