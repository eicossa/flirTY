#ifndef SEGMENTOR_H
#define SEGMENTOR_H

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#include <QObject>
#include <QAbstractButton>
#include "segmentorparams.h"

class Segmentor : public QObject
{
    Q_OBJECT
signals:
    void signalSegmentorProgress(int);
public:
    Segmentor();
    void loadThermalImage(std::string imgpath, int flags);
    void calculateBlobAreas();
    void segmentImage();

    void                              updateWithSelectedBlobAreas(int, int);
    //void                              filterBlobsByArea(int, int);
    int                               getMinArea();
    int                               getMaxArea();
    std::vector<Vec3b>                getColors();
    std::vector<int>                  getBlobAreas();
    std::vector<int>                  getSortedBlobAreas();
    std::vector<int>                  getSelectedBlobAreas();
    std::vector<std::vector<Point2i>> getBlobPoints();
    std::vector<Point2i>              getIsPointKaBlob(QPointF p);



    Mat  getOriginalMat()        { return src;              }
    Mat  getFilteredMat()        { return imgFiltered;      }
    Mat  getMorphologizedMat()   { return imgMorphologized; }
    Mat  getBinaryMat()          { return bw;               }
    Mat  getDistTransformedMat() { return distTransformed;  }
    Mat  getPeaksMat()           { return peaks;            }
    Mat  getMarkersMat()         { return markers;          }
    Mat  getFinalMat()           { return final;            }
    Mat  getFinalMat2()          { return final2;           }
    Mat  getSelectedMat()        { return selected;         }

    SegmentorParams* params()    { return sParams;          }
public slots:

private:
    Mat  src;
    void changeBackgroundImage();
    Mat  imgFiltered;
    void filterImage();

    int morphOperator;
    Mat imgMorphologized;
    void morphologizeImage();


    void binarizeImage();
    Mat bw;
    void distanceTransformImage();
    Mat distTransformed;
    void obtainPeaksImage();
    Mat peaks;
    void watershedImage();
    Mat markers;
    int contourSize;
    void colorizeImageSegments();
    void generateRandomColors();
    void initializeBlobAreasBlobPoints();
    void fillRandomColors();
    Mat final;
    Mat selected;

    Mat final2;

    SegmentorParams *sParams;

    std::vector<std::vector<Point>> contours;
    std::vector<std::vector<Point>> finalcontours;

    int minArea, maxArea, numOfBlobs;

    std::vector<Vec3b>                           colors;
    std::vector<int>                             blobAreas;
    std::vector<int>                             sortedBlobAreas;
    std::vector<int>                             selectedBlobAreas;
    std::vector<int>                             selectedBlobIndices;
    std::vector<std::vector<Point2i>>            blobPoints;

    void                              generateSortedBlobAreas();
    void                              generateSelectedBlobAreas(int, int);
    void                              generateSelectedBlobIndices();
    void                              fillSelectedAreasRandomColors();
};
#endif // SEGMENTOR_H
