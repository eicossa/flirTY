#ifndef FLIRIMGMETADATA_H
#define FLIRIMGMETADATA_H
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <cassert>
#include <opencv2/opencv.hpp>
//#include "ExifTool.h"
#include "utilfunc.h"
#include "cmdstrings.h"

using namespace std;
using namespace cv;

#include <QObject>


class flirImgMetadata : public QObject
{
   Q_OBJECT
signals:
  void signalFlirImgMetadataProgress(int);
private:
  std::string        imgpath;
  std::string        therm_imgpath;
  std::string        metadataFile;

  shellExecutor      *shX;
  // read thermal metadata
  double             planckr1, planckr2, planckb, planckf, plancko;
  double             tref, emis;
  double             rawvaluerange, rawvaluemedian;
  // read basic metadata
  int                imagewidth, imageheight;
  int                rawthermalimagewidth, rawthermalimageheight;
  // calculated values
  double             rawmin, rawmax, rawrefl;
  double             rawminobj, rawmaxobj;
  double             tmin, tmax;
  double             smin, smax, sdelta;

  // progress level
  int                pLevel;

  // the workhorse image reader
  double readExiftoolTagInfo(std::string tagname);

  //reading methods
  void readThermalMetadata();
  void readBasicMetadata();

  // checking methods
  void checkReadMetadata();
  void checkCalcedMetadata();

  void extractRawThermal();

  // calculation methods
  void               calcRAWmax();
  void               calcRAWmin();
  void               calcRAWrefl();
  void               calcRAWminobj();
  void               calcRAWmaxobj();
  void               calcRaws();

  void               calcTmin();
  void               calcTmax();
  void               calcTs();

  void               calcSmin();
  void               calcSmax();
  void               calcSdelta();
  void               calcSes();

  void               readAllMetadata();
  void               calcAllMetadata();

  // set progress levels
  void               setProgressLevel(int p) { pLevel = p; }

public:
  flirImgMetadata();
  void loadMetadata(std::string imgpath, std::string thermimgpath);

  void    loadPrereadMetadata();
  void    writePrereadMetadata();

  double calcTempForOnePixel(double thermalintensityvalue);

  void    printCalcedMetadata();
  void    printReadMetadata();
  void    printImageSummary();

  int     getRAWThermalImageWidth(){return rawthermalimagewidth;}
  int     getRAWThermalImageHeight(){return rawthermalimageheight;}
  int     getOrigImageWidth() {return imagewidth;}
  int     getOrigImageHeight() {return imageheight;}

  int     getProgressLevel() { return pLevel; }
};

#endif // FLIRIMGMETADATA_H




