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
#include "shellexecutor.h"

using namespace std;
using namespace cv;

#include <QObject>

class flirImgTags{


};


class flirImgMetadata : public QObject
{
   Q_OBJECT
signals:
  void signalFlirImgMetadataProgress(int);
private:
  std::string        imgpath;
  std::string        therm_imgpath;






  // progress level
  int                pLevel;



  // the workhorse image reader
  double readExiftoolTagInfo(std::string tagname);

  //reading methods

  //void readBasicMetadata();

  // checking methods
  void checkReadMetadata();

  void               readAllMetadata();
  void               calcAllMetadata();

  // set progress levels
  void               setProgressLevel(int p) { pLevel = p; }

public:
  flirImgMetadata();
  void loadMetadata(std::string imgpath, std::string thermimgpath);
  QMap<QString, QString> readAllMetadataTags();
  void extractRawThermal();
  int     getProgressLevel() { return pLevel; }
};

#endif // FLIRIMGMETADATA_H




