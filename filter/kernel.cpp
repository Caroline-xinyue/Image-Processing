#include "kernel.h"

using namespace cv;

Kernel::Kernel (int a1, int a2, int b1, int b2)
{
  kern = (Mat_<char>(2,2) << a1, a2,
		             b1, b2);
}

Kernel::Kernel (int a1, int a2, int a3, int b1, int b2, int b3, int c1, int c2, int c3) {
  kern = (Mat_<char>(3,3) << a1, a2, a3,
		             b1, b2, b3,
                             c1, c2, c3);
}

Kernel::Kernel (int a1, int a2, int a3, int a4, int a5, int b1, int b2, int b3, int b4, int b5, int c1, int c2, int c3, int c4, int c5, int d1, int d2, int d3, int d4, int d5, int e1, int e2, int e3, int e4, int e5) {
  kern = (Mat_<char>(5,5) << a1, a2, a3, a4, a5,
	                     b1, b2, b3, b4, b5,
	                     c1, c2, c3, c4, c5,
	                     d1, d2, d3, d4, d5,
	                     e1, e2, e3, e4, e5);
}

/*
  Filter the image matrix based on the kernel selected
  Reference:
  http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/filter_2d/filter_2d.html
 */
void Kernel::filter(Mat src, Mat dst) {
  filter2D(src, dst, -1 , kern, Point(-1, -1), 0, BORDER_DEFAULT);
}
