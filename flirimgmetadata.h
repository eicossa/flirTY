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
<<<<<<< HEAD
#include "shellexecutor.h"
=======
#include "cmdstrings.h"
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

using namespace std;
using namespace cv;

#include <QObject>

<<<<<<< HEAD
class flirImgTags{


};

=======
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

class flirImgMetadata : public QObject
{
   Q_OBJECT
signals:
  void signalFlirImgMetadataProgress(int);
private:
  std::string        imgpath;
  std::string        therm_imgpath;
<<<<<<< HEAD





=======
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
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

  // progress level
  int                pLevel;

<<<<<<< HEAD


=======
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
  // the workhorse image reader
  double readExiftoolTagInfo(std::string tagname);

  //reading methods
<<<<<<< HEAD

  //void readBasicMetadata();

  // checking methods
  void checkReadMetadata();
=======
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
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

  void               readAllMetadata();
  void               calcAllMetadata();

  // set progress levels
  void               setProgressLevel(int p) { pLevel = p; }

public:
  flirImgMetadata();
  void loadMetadata(std::string imgpath, std::string thermimgpath);
<<<<<<< HEAD
  QMap<QString, QString> readAllMetadataTags();
  void extractRawThermal();
=======

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

>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
  int     getProgressLevel() { return pLevel; }
};

#endif // FLIRIMGMETADATA_H




