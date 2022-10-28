#ifndef ERRMSGS_H
#define ERRMSGS_H
#include <iostream>
#include "imgproc/opencv_headers.h"
#include <string>
using namespace std;
using namespace cv;

class ErrMsgs{
public:
  ErrMsgs();
  std::string getRGBimgErrorMsg();
  std::string getThermalImgErrorMsg();
  std::string getPHPimgErrorMsg();
};


#endif // ERRMSGS_H
