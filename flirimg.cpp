#include "flirimg.h"
#include "cmdstrings.h"
using namespace cv;

flirImg::flirImg()
{
  fmd     = NULL;
  imgpath = "";
  thermal_imgpath ="";
  emsgs   = new ErrMsgs();
  temperatures = NULL;

  fmd          = new flirImgMetadata();
}


void flirImg::loadOrigImage(std::string imgpath)
{

  this->imgpath            = imgpath;
  this->thermal_imgpath    = imgpath + std::string(".thermout.png");
  this->binary_imgpath     = imgpath + std::string(".bindata");
  this->repaletted_imgpath = imgpath + std::string(".repaletted.png");

  fmd->loadMetadata(imgpath, this->thermal_imgpath);
}

void flirImg::printImageSummary()
{
  fmd->printImageSummary();
}

bool flirImg::didThermalConversionSucceed()
{
    //originalMat = (fimg->getThermalImgMat()).clone();
    cv::Mat originalMat = imread(thermal_imgpath, cv::IMREAD_COLOR);

    if(!originalMat.empty()) {
      return true;
    } else {
      return false;
    }
}

void flirImg::loadRAWThermalImage()
{
    // read image from file
    thermimg = imread(thermal_imgpath, IMREAD_ANYDEPTH);
    cout<<thermimg.size()<<endl;
    cout<<thermimg.type()<<endl;
    cout<<thermimg.channels()<<endl;

    // if fail to read the image
    if ( thermimg.empty() ) {
      cout << emsgs->getThermalImgErrorMsg() << thermal_imgpath << endl;
      exit;
    } else{
      cout << "Successfully read in "
       << thermal_imgpath << endl;
    }
}

double flirImg::calcBlobAvgTemp(std::vector<Point2i> blobPts)
{
    double avgBlobTemp    = 0.0;
    double sumOfBlobTemps = 0.0;
    for(int i = 0; i < blobPts.size(); i++){
        Point2i pt = blobPts.at(i);
        int y = pt.y;
        int x = pt.x;
        if(x!=-1 && y!=-1)
          sumOfBlobTemps += temperatures.at<double>(x, y);
    }
    int numOfPoints = blobPts.size();
    avgBlobTemp = sumOfBlobTemps/numOfPoints;
    return avgBlobTemp;
}

void flirImg::calcTempForEveryPixel()
{
  // this->getMetadata()->checkPlancks();
  // this->getMetadata()->checkSmin();
  // this->getMetadata()->checkSdelta();

  int thermalvalue;
  double calculatedTemp;

  int rawthermalimgwidth  = this->getMetadata()->getRAWThermalImageWidth();
  int rawthermalimgheight = this->getMetadata()->getRAWThermalImageHeight();

  cout << "flirimg:calctemp4everypixel: Raw Thermal Width  " << rawthermalimgwidth  << endl;
  cout << "flirimg:calctemp4everypixel: Raw Thermal Height " << rawthermalimgheight << endl;
  temperatures = Mat::zeros(this->thermimg.size(), CV_64F);

  for(int i=0; i<rawthermalimgwidth; i++){
    for(int j=0; j<rawthermalimgheight; j++){
      // cout << " (i, j) : " << i << ", " << j << " | ";
      thermalvalue = (int)((this->thermimg).at<unsigned short>(j, i));
      // cout << thermalvalue << endl;
      calculatedTemp = this->getMetadata()->calcTempForOnePixel(thermalvalue);
      temperatures.at<double>(j, i) = calculatedTemp;
      // emit signalFlirImgProgress(((i*j)/(rawthermalimgwidth*rawthermalimgheight))*100);
      // cout << temperatures.at<double>(j, i) << endl;
    }
    double progressLevel = (double)((double)i/(double)rawthermalimgwidth);
    progressLevel     = progressLevel*100;
    // cout << "Progress Level : " << progressLevel << endl;

    emit signalFlirImgProgress((int)progressLevel);
  }

  emit signalFlirImgProgress(100);
  cout << " Done with temp calculations " << endl;
}

void flirImg::temperatureCallBackFunc(int event, int x, int y, int flags, void* param)
{
  double          calculatedTemp;
  flirImg*        flirImgObject;
  flirImgObject = (flirImg*) param;
  Mat             pixelTemps;

  pixelTemps    = flirImgObject->getPixelTemperatures();

  if( event == EVENT_LBUTTONDOWN ){
    calculatedTemp = pixelTemps.at<double>(y, x);
    cout << calculatedTemp << endl;
  }
  else if  ( event == EVENT_RBUTTONDOWN )
  {
    // cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    // print50PixelsDiagnostics(flirImgParam);
  }
  else if  ( event == EVENT_MBUTTONDOWN )
  {
    // cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  }
  else if ( event == EVENT_MOUSEMOVE )
  {
    // cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
    calculatedTemp = pixelTemps.at<double>(y, x);
    // cout << calculatedTemp << endl;
  }
}

double flirImg::getPixelTemperature(int x, int y)
{
    // cout<<"Tryin to get temp at ("<<x<<", "<<y<<" )"<<endl;
    double temp = temperatures.at<double>(y, x);
    return temperatures.at<double>(y, x);
}
