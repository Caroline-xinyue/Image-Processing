#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <bitset>

using namespace cv;

int readImage(std::string);
std::bitset<8> decTobin(uchar);
void decode();


Mat src;
int numRows = 0;
int numColumns = 0;
int numColors = 0;
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
  numRows = src.rows;
  numColumns = src.cols;
  numColors = src.channels();
  return 0;
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
  Text decode algorithm: Similar to the text encode algorithm, the text decode algorithm is basically an inverse operation of the text encode algorithm. First convert the 3 color channel values of each pixel to 8 bit binary. Then we read the 21 bit binary from the B, G, R channels of the first 9 pixels and use the to_ulong method to convert it back to int. Finally, keep reading in the 8 bit binary from the B, G, R channels of three pixels and convert each to uchar and print to the console. Once the count is larger than the msglen, the function returns and the operation finishes.

  References:
  http://docs.opencv.org/2.4/doc/user_guide/ug_mat.html
  http://www.cplusplus.com/reference/bitset/bitset/to_ulong/
 */
void decode()
{
  std::bitset<8> b,g,r,m;
  std::bitset<21> l;
  int k = 20;
  int count = 0;
  uchar msg;
  int msglen = 1;
  
  for(int i = 0; i < numColumns; i++) {
    for(int j = 0; j < numRows; j++) {
      if(count > msglen) {
	std::cout << std::endl;
	return;
      }
      
      Vec3b data = src.at<Vec3b>(i, j);
      b = decTobin(data[0]);
      g = decTobin(data[1]);
      r = decTobin(data[2]);

      if(count == 0) {
	l[k--] = b[0];
	l[k--] = b[1];
	l[k--] = b[2];
	if(k == -1) {
	  msglen = (int)l.to_ulong();
	  k = 7;
	  count++;
	  continue;
	}
      } else {
	m[k--] = b[0];
      }
      
      if(count == 0) {
	l[k--] = g[0];
	l[k--] = g[1];
	l[k--] = g[2];
      } else {
	m[k--] = g[0];
	if(k == -1) {
	  msg = static_cast<uchar>(m.to_ulong());
	  k = 7;
	  std::cout << msg;
	  count++;
	  continue;
	}
      }
      if(count == 0) {
	l[k--] = r[0];
	l[k--] = r[1];
	l[k--] = r[2];
      } else {
	m[k--] = r[0];
      }
    }
  }
}
