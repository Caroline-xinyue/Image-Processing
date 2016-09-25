#ifndef KERNEL_H
#define KERNEL_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Kernel {
  cv::Mat kern;
  public:
    Kernel (int,int,int,int);
    Kernel (int,int,int,int,int,int,int,int,int);
    Kernel (int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int);
    void filter(cv::Mat src, cv::Mat dst);
};
#endif
