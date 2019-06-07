#ifndef SEGMENTOR_H
#define SEGMENTOR_H

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#include <QObject>
#include <QAbstractButton>
#include "segmentorparams.h"
<<<<<<< HEAD
#include "flirimg.h"
=======
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

class Segmentor : public QObject
{
    Q_OBJECT
signals:
    void signalSegmentorProgress(int);
public:
    Segmentor();
<<<<<<< HEAD
    void loadDisplayRAWsImage(flirImg *f, int flags);
=======
    void loadThermalImage(std::string imgpath, int flags);
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
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
<<<<<<< HEAD
    int                               getIsPointKaBlobArea(QPoint p);
    int                               getIsPointKaBlobID(QPoint p);
    double                            getIsPointKeBlobKaAvgTemp(QPoint p);
    std::vector<Point2i>              getIsPointKeBlobKePoints(QPoint p);
    std::map<Point2i, int>            pointBlobIDMap;
=======
    std::vector<Point2i>              getIsPointKaBlob(QPointF p);
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3



    Mat  getOriginalMat()        { return src;              }
    Mat  getFilteredMat()        { return imgFiltered;      }
    Mat  getMorphologizedMat()   { return imgMorphologized; }
    Mat  getBinaryMat()          { return bw;               }
    Mat  getDistTransformedMat() { return distTransformed;  }
    Mat  getPeaksMat()           { return peaks;            }
    Mat  getMarkersMat()         { return markers;          }
<<<<<<< HEAD
    Mat  getSegmentedColoredMat()           { return segmentedColoredMat;            }
    Mat  getFinalMat2()          { return final2;           }
    Mat  getSelectedMat()        { return selected;         }
    QString getStatusString(QPoint);
=======
    Mat  getFinalMat()           { return final;            }
    Mat  getFinalMat2()          { return final2;           }
    Mat  getSelectedMat()        { return selected;         }
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

    SegmentorParams* params()    { return sParams;          }
public slots:

private:
<<<<<<< HEAD
    flirImg *fimg;
=======
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
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
<<<<<<< HEAD
    Mat segmentedColoredMat;
=======
    Mat final;
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
    Mat selected;

    Mat final2;

    SegmentorParams *sParams;

    std::vector<std::vector<Point>> contours;
    std::vector<std::vector<Point>> finalcontours;

    int minArea, maxArea, numOfBlobs;

<<<<<<< HEAD
    std::vector<Vec3b>                           blobIDcolors;
=======
    std::vector<Vec3b>                           colors;
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
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
