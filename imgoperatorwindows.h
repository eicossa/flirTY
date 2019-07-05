#ifndef IMGOPERATORWINDOWS_H
#define IMGOPERATORWINDOWS_H

#include "segmentor.h"
#include "flirimg.h"
#include "ui/segmentorwindow.h"
#include "ui/flirimgwindow.h"

class imgOperatorWindows : public QObject
{
    Q_OBJECT
public slots:
    void            isPointKeBlobKaTempNikaaloMadarchod(QPoint p);
    void            itsTimeToDisplayTheBlobOverlaidImage(bool);
    void            emitBlobAvgTempMilaBhosadiwaale(double);
signals:
    void            blobAvgTempLeleBhosdike(double);
public:
    explicit imgOperatorWindows(QWidget *parent = 0);
    ~imgOperatorWindows() {};

    void            buildFlirImgObjAndSegmentorObj(QString imgPath);
    void            buildShitForThisImgpath(QString);
    void            connectEverything();

    flirImg*         getFlirImg()         { return this->fimg;      }
    Segmentor*       getSegmentor()       { return this->segmentor; }
    flirImgWindow*  getFlirbabaWindow()  { return this->fW; }
    //segmentorWindow* getSegmentorWindow() { return this->sW; }

private:
    int                      numOfBlobs;
    flirImgWindow           *fW;
    //segmentorWindow          *sW;
    flirImg                  *fimg;
    Segmentor                *segmentor;
    cv::Mat                  originalMat;
    cv::Mat                  overlayMat;
};

#endif // IMGOPERATORWINDOWS_H
