#ifndef SEGMENTOR_H
#define SEGMENTOR_H

#include "opencv_headers.h"

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
    void initSegmentor(Mat src);
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
    int                               getIsPointKaBlobArea(QPoint p);
    int                               getIsPointKaBlobID(QPoint p);
//    double                            getIsPointKeBlobKaAvgTemp(QPoint p);
    std::vector<Point2i>              getIsPointKeBlobKePoints(QPoint p);
    std::map<Point2i, int>            pointBlobIDMap;

    int  getNumOfBlobs()         { return numOfBlobs; }

    Mat  getOriginalMat()        { return src;              }
    Mat  getFilteredMat()        { return imgFiltered;      }
    Mat  getMorphologizedMat()   { return imgMorphologized; }
    Mat  getBinaryMat()          { return bw;               }
    Mat  getDistTransformedMat() { return distTransformed;  }
    Mat  getPeaksMat()           { return peaks;            }
    Mat  getMarkersMat()         { return markers;          }
    Mat  getSegmentedColoredMat()           { return segmentedColoredMat;            }
    Mat  getFinalMat2()          { return final2;           }
    Mat  getOverlayMat()        { return selected;         }
    QString getStatusString(QPoint);

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
    Mat segmentedColoredMat;
    Mat selected;

    Mat final2;

    SegmentorParams *sParams;

    std::vector<std::vector<Point>> contours;
    std::vector<std::vector<Point>> finalcontours;

    int minArea, maxArea, numOfBlobs;

    std::vector<Vec3b>                           blobIDcolors;
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
