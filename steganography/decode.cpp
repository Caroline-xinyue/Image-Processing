#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <bitset>

using namespace cv;

int readImage(std::string);
void binTodec(bool*);
void decode();
void getMsglen();

Mat src;
int numRows = 0;
int numColumns = 0;
int numColors = 0;
int numPixels = 0;
uchar* data;
uchar secret;
int msglen = 0;

int main( int argc, char** argv)
{
  int invalid = readImage("secretMessage.png");
  if(invalid) {std::cout << "Error loading image \n"; return -1;}
  if(numColors != 3) {
    std::cout << "Please upload a 3-channel color image \n";
    return -1;
  }

  decode();

  waitKey(0);
  return 0;
}

int readImage(std::string path)
{
  src = imread(path,CV_LOAD_IMAGE_COLOR);
  
  if(!src.data)
    {
      std::cerr << "Error loading image \n";
      return -1;
    }
  data = src.data;
  numRows = src.rows;
  numColumns = src.cols;
  numColors = src.channels();
  numPixels = numColumns * numRows;
  return 0;
}

/*
  Change every 8 bit binary number into the corresponding character and prints them out
 */
void binTodec(bool* bits)
{
  uchar text = 0;
  uchar bit = 0;
  
  for(int i = 0; i < 8 * msglen; i+=8) {
    text = 0;
    for(int j = 7; j >= 0; j--) {
      bit = bits[i + j];
      text = text * 2 + bit;
    }
    std::cout << text;
  }
}

/*
  Text decode algorithm: Similar to the text encode algorithm, the text decode algorithm is basically an inverse operation of the text encode algorithm. It first calls getMsglen() to read in the first 32 data values to initialize the msglen integer value, then it reads in the least significant bit of each data value form the encoded image file and stores them into a bool array of bits. Finally it calls the binTodec(bool*) method and converts every 8 of them to the text character and prints them out.
*/

void decode()
{
  int n = 0;
  getMsglen();
  int bitSize = 8 * msglen;
  bool bits[bitSize];// the bool array of bits equivalent to the input text array

  for(int i = 32; i < numPixels * 3; i++) {
    if(n >= bitSize) {
      break;
    }
    bits[n++] = (data[i] % 2);
  }

  binTodec(bits);
  std::cout << std::endl;
}

/*
  reads in the first 32 data values where the length of the input text message is stored
 */
void getMsglen()
{
  uchar bit = 0;
  for(int k = 31; k >= 0; k--) {
    bit = data[k] % 2;
    msglen = msglen * 2 + bit;
  }
}
