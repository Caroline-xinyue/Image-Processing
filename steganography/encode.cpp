#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <bitset>

using namespace cv;

int readImage(std::string);
void writeImage(std::string);
void showImage(string, Mat&);
void showImages();
void decTobin(uchar*);
void encode(uchar*);

Mat src, dst;
int numRows = 0;
int numColumns = 0;
int numColors = 0;
int numPixels = 0;
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

  uchar text[numPixels];
  
  // Read in secret text
  
  while(std::cin >> std::noskipws >> secret)
    {
      text[msglen++] = secret;
      if(msglen >= (3*numPixels - 32)/8){
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
  numPixels = numColumns * numRows;
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
  Change the decimal unsigned int number to its corresponding 8 bit binary number.
 */
void decTobin(uchar* text, bool* bits)
{
  int k = 0;
  uchar character = 0;
  int len = msglen;
  // stores the message length into bits
  for(int i = 0; i < 32; i++) {
    bits[k++] = len % 2;
    len /= 2;
  }
  // stores each text character into bits
  for(int i = 0; i < msglen; i++) {
    character = text[i];
    for(int j = 0; j < 8; j++) {
      bits[k++] = character % 2;
      character /= 2;
    }
  }
}

/*
  Text encode algorithm: First call the decTobin method to convert the ascii caracter arrays of texts to the corresponding bool array of text bits. Then write these text bits to the least significant bit of the binary representation of the image data. Since the least significant bit is either 0 or 1, I modify it unless this bit and the text bits are the same (aka either all even or all odd). Since overall, each channel of the pixel value only change by 1, it is guaranteed to be not noticeable by human eyes.

  References:
  http://docs.opencv.org/2.4/doc/user_guide/ug_mat.html
 */

void encode(uchar* text)
{
  int n = 0;
  uchar bit = 0; // the bit representations of the input text
  int bitSize = 8 * msglen + 32;
  bool bits[bitSize];// the bool array of bits equivalent to the msglen followed by the characters in the input text array
  decTobin(text, bits);
  
  for(int i = 0; i < numPixels * 3; i++) {
    if(n >= bitSize) {
      return;
    }
    bit = bits[n++];
    if(data[i] % 2 != bit) {
      (data[i] % 2 > bit) ? data[i] -= 1 : data[i] += 1;
    }
  }
}
