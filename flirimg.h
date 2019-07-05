#ifndef FLIRIMG_H
#define FLIRIMG_H
#include <iostream>
#include <cmath>
#include <string>
#include <stdio.h>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

#include <QMap>
#include <QObject>
#include "errmsgs.h"
#include "segmentor.h"
//#include "bezier.hpp"

using namespace std;
using namespace cv;

class flirImg : public QObject
{
  Q_OBJECT
signals:
  void signalFlirImgProgress(int);
private:
  QMap<QString, QString> flirTagsMap;
  ErrMsgs*               emsgs;

  QString                filename;
  QString                imgpath;
  QString                unorderedRAWs_imgpath, orderedRAWs_imgpath;
  QString                displayOrderedRAWs_imgpath, displayUnorderedRAWs_imgpath;
  QString                rawpalette_imgpath, palette_imgpath;
  QString                linearGrayscaleDisplay_imgpath;

  //std::string          reorderedRAWs_imgpath;
  QString                repalettedOrderedRAWs_imgpath;
  QString                repalettedUnorderedRAWs_imgpath;
  QString                embeddedRGB_imgpath;
  //std::string          preorderedRAWs_resized_imgpath;

  //std::string          tempsFile1_interpolated;
  QString                tempsCSVFile;
  QString                orderedRAWsCSVFile;
  QString                unorderedRAWsCSVFile;

  QString                metadataFile;
  QString                fileName;

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
  // camera details
  std::string        RawThermalImageType;
  std::string        cmodel;
  // palette details
  std::string        palName;
  int                paletteColors;

  // matrices associated with the image
  Mat                 origImgMat;
  Mat                 unorderedRAWsMat, orderedRAWsMat;
  Mat                 repalettedImgMat;
  Mat                 rawpalette_imgMat, palette_imgMat;
  Mat                 overlaidMat;
  Mat                 temperatures;
  Mat                 normalizedThermalDisplayMat;
  Mat                 grayscaleThermalDisplayMat;
  // image processors for the image
  Segmentor           *segmentor;

  void initializePaths(std::string imgpath);

  // zzzzzzzzzzzzzzzzzzz
  enum convertArgTypes { ORDEREDRAWS_2_DISPLAYPNG, UNORDEREDRAWS_2_DISPLAYPNG, RAWPALETTE_2_1DPALETTE};
  enum extractArgTypes { ALLTAGS, UNORDEREDRAWS, PALETTE };
  enum cmdExecType     { start, execute };
  bool               readFileIntoMat(QString, Mat&, cv::ImreadModes);
  QString            transformToWindowsPath(QString);

  // calculation methods
  void               calcRAWrefl();
  void               calcTminTmax();

  QStringList        buildConvertArgList(convertArgTypes t);
  QStringList        buildExtractionArgList(extractArgTypes t);
  void               executeZeCommand(QString, QStringList, QString&, QString&, cmdExecType);
  void               generateGrayscaleImage();

  bool               doesFileExist (const QString&);
  unsigned short     changeByteOrder(unsigned short);
  void               updateSegmentorObjDetails();

public:
  flirImg();
  flirImg(QString);
  void                processImage(std::string fimgpath);
  void                printImageSummary();
  void                extractThermalRaw();
  void                displayTooltippedImage();

  void                calcTempForEveryPixel();
  double              calcBlobAvgTemp(std::vector<Point2i>);
  void                applyPalette();

  QString             getStatusString(int, int);

  double              calcRawObjForRawThermal(double);
  double              calcTempFromRawObjValue(double);
  double              calcTempForOnePixel(double);

  bool                writeMatToFile(QString, Mat);


  void                segmentImage();
  Mat                 getPixelTemperatures(){return temperatures;}
  double              getPixelTemperature(int x, int y);
  int                 getOrderedRAWval(int x, int y);
  int                 getExtThermalValue(int x, int y);

  QString             getImgPath()                      { return imgpath; }
  QString             getUnorderedRAWsImgpath()         { return unorderedRAWs_imgpath; }
  QString             getDisplayOrderedRAWsImgpath()    { return displayOrderedRAWs_imgpath; }
  QString             getRepalettedOrderedRAWsImgPath() { return repalettedOrderedRAWs_imgpath; }


  Mat                 getUnorderedRAWsImgMat()          { return unorderedRAWsMat;         }
  Mat                 getOrderedRAWsImgMat()            { return orderedRAWsMat;           }
  Mat                 getOrigImgMat()                   { return origImgMat;               }
  Mat                 getGrayscaleThermalMat()          { return grayscaleThermalDisplayMat;}

  Mat                 getRepalettedImgMat()             { return repalettedImgMat;  }

  Mat                 getTransparencyOverlaidMat(double);


  bool                didUnorderedRAWsExtractionSucceed();
  bool                didDisplayRAWsConversionSucceed();

  void                writeRaw2TempValues();

  int                 getRAWThermalImageWidth(){return rawthermalimagewidth;}
  int                 getRAWThermalImageHeight(){return rawthermalimageheight;}
  int                 getOrigImageWidth() {return imagewidth;}
  int                 getOrigImageHeight() {return imageheight;}

  Segmentor*          getSegmentorObject() { return segmentor; }

  void                printCalcedMetadata();
  void                printReadMetadata();

  void    loadPrereadMetadata();
  QVector<QStringList>    readImgdataCsvFile(QString);
  void                writeImgdataCsvFile(QString, QVector<QStringList>);
  void                writePrereadMetadata();

  void                changeByteOrderOfRAWsMat();
  void                normalizeTempMat();

  bool                extractRAWs();
  void                extractAllTags();

  QString     getExiftoolPath();
  QString     getConvertPath();
  bool        convertOrderedRAWsToDisplayPNG();
  bool        convertUnorderedRAWsToDisplayPNG();

  bool        extractEmbeddedRGBImage();
  bool        extractPalette();
  bool        executeRepaletteImage();

  void displayUsingOpenCV(std::string, Mat);
  void changePalette(ColormapTypes);

  QString     getFileName();

};

#endif // FLIRIMG_H
