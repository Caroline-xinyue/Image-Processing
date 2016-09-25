#ifndef FILTER_H
#define FILTER_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "kernel.h"

  Kernel kern2(0, 1, 1, 0);
  Kernel kern3_1(0, 2, -2, 2, -1, 2, 0, 2, -2);
  Kernel kern3_2(-1, 5, -1, 0, -5, 0, -1, 5, -1);
  Kernel kern3_3(1, -1, 1, -1, 1, -1, 1, -1, 1);
  Kernel kern5(1, 2, -3, 2, 1, -1, -2, 3, -2, -1, 1, 2, -3, 2, 1, -1, -2, 3, -2, -1, 1, 2, -3, 2, 1);

#endif
