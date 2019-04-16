#ifndef FLIRIMG_H
#define FLIRIMG_H
#include <iostream>
#include <cmath>
#include <string>
#include <stdio.h>

#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "flirimgmetadata.h"
#include "errmsgs.h"
//#include "bezier.hpp"

using namespace std;
using namespace cv;

class flirImg : public QObject
{
  Q_OBJECT
signals:
  void signalFlirImgProgress(int);
private:
  flirImgMetadata*    fmd;
  ErrMsgs*            emsgs;
  std::string         imgpath;
  std::string         binary_imgpath;
  std::string         thermal_imgpath;
  std::string         repaletted_imgpath;
  Mat                 thermimg;
  Mat                 temperatures;
  void  temperatureCallBackFunc(int, int, int, int, void*);
  void changeBackgroundImage(Mat&);
  void filterImage(Mat, Mat&);
  void binarizeImage(Mat&);
  void distanceTransformImage(Mat, Mat&);
  void obtainPeaksImage(Mat&);
  void generateWatershedMarkers(Mat, Mat&, int&);
  void generateFinalResultImage(int, Mat, Mat&);
public:
  flirImg();
  void loadOrigImage(std::string fimgpath);
  void printImageSummary();
  flirImgMetadata* getMetadata(){return this->fmd;}
  void loadRAWThermalImage();
  void extractThermalRaw();
  void displayTooltippedImage();

  void   calcTempForEveryPixel();
  double calcBlobAvgTemp(std::vector<Point2i>);

  void   segmentImage();
  Mat    getPixelTemperatures(){return temperatures;}
  double getPixelTemperature(int x, int y);
  std::string getImgPath()        { return imgpath; }
  std::string getBinaryImgPath()  { return binary_imgpath; }
  std::string getThermalImgPath() { return thermal_imgpath; }
  std::string getRepalettedImgPath() { return repaletted_imgpath; }
  Mat    getThermalImgMat(){return thermimg;}
  bool didThermalConversionSucceed();
};

#endif // FLIRIMG_H
