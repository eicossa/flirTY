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
  int printCounter;
  QMap<QString, QString> flirTagsMap;
  flirImgMetadata*    fmd;
  ErrMsgs*            emsgs;

  std::string         imgpath;
  std::string         unorderedRAWs_imgpath;
  std::string         displayRAWs_imgpath;
  std::string         displayRGB_imgpath;


  std::string         reorderedRAWs_imgpath;
  std::string         repaletted_imgpath;
  std::string         preorderedRAWs_resized_imgpath;

  std::string         tempsFile1_interpolated;
  std::string         tempsFile;
  std::string         orderedRAWsFile;
  std::string         unorderedRAWsFile;

  std::string         metadataFile;

  // read thermal metadata
  double             planckr1, planckb, planckf, plancko, planckr2;
  double             tref, emis;
  double             ata1, ata2, atb1, atb2, atx;
  double             atmTemp, IRWTemp, IRWTrans, relHum;
  double             rawvaluerange, rawvaluemedian;
  double             focusDist, objDist;
  // read basic metadata
  int                imagewidth, imageheight;
  int                rawthermalimagewidth, rawthermalimageheight;
  // calculated values
  double             rawMin, rawMax, rawRefl;
  double             rawMinObj, rawMaxObj;
  double             tmin, tmax;
  double             smin, smax, sdelta;
  // camera details
  std::string        RawThermalImageType;
  std::string        palName;
  std::string        cmodel;

  // matrices associated with the image
  Mat                 unorderedRAWsMat;
  Mat                 orderedRAWsMat;
  Mat                 displayRAWsMat;
  Mat                 displayRGBMat;
  Mat                 displayRepalettedMat;

  Mat                 temperatures;

  shellExecutor      *shX;

  void initializePaths(std::string imgpath);
  void readAllTags();

  // calculation methods
  void               calcRAWrefl();
  void               calcTminTmax();

public:
  flirImg();
  void                processImage(std::string fimgpath);
  void                printImageSummary();
  flirImgMetadata*    getMetadata(){return this->fmd;}
  void                extractThermalRaw();
  void                displayTooltippedImage();

  void                calcTempForEveryPixel();
  double              calcBlobAvgTemp(std::vector<Point2i>);

  QString             getStatusString(int, int);

  double calcRawObjForRawThermal(double);
  double calcTempFromRawObjValue(double);
  double calcTempForOnePixel(double);


  void                segmentImage();
  Mat                 getPixelTemperatures(){return temperatures;}
  double              getPixelTemperature(int x, int y);
  int                 getOrderedRAWval(int x, int y);
  int                 getExtThermalValue(int x, int y);
  std::string         getImgPath()        { return imgpath; }
  std::string         getUnorderedRAWsImgpath()  { return unorderedRAWs_imgpath; }
  std::string         getDisplayRAWsImgpath() { return displayRAWs_imgpath; }
  std::string         getRepalettedImgPath() { return repaletted_imgpath; }
  Mat                 getUnorderedRAWsImgMat(){return unorderedRAWsMat;}
  Mat                 getDisplayRAWsImgMat()  { return displayRAWsMat; }


  bool                didUnorderedRAWsExtractionSucceed();
  bool                didDisplayRAWsConversionSucceed();

  void                writeRaw2TempValues();

  int                 getRAWThermalImageWidth(){return rawthermalimagewidth;}
  int                 getRAWThermalImageHeight(){return rawthermalimageheight;}
  int                 getOrigImageWidth() {return imagewidth;}
  int                 getOrigImageHeight() {return imageheight;}

  void                printCalcedMetadata();
  void                printReadMetadata();

  void    loadPrereadMetadata();
  QVector<QStringList>    readImgdataCsvFile(QString);
  void writeImgdataCsvFile(QString, QVector<QStringList>);
  void    writePrereadMetadata();

  void    createOrderedRAWsMat();
};

#endif // FLIRIMG_H
