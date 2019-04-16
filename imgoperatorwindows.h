#ifndef IMGOPERATORWINDOWS_H
#define IMGOPERATORWINDOWS_H

#include "segmentor.h"
#include "flirimg.h"
#include "segmentorwindow.h"
#include "flirbabawindow.h"

class imgOperatorWindows : public QObject
{
    Q_OBJECT
public slots:
    void            isPointKeBlobKaTempNikaaloMadarchod(QPointF p);
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
    flirbabaWindow*  getFlirbabaWindow()  { return this->fW; }
    segmentorWindow* getSegmentorWindow() { return this->sW; }

private:
    int                      numOfBlobs;
    flirbabaWindow           *fW;
    segmentorWindow          *sW;
    flirImg                  *fimg;
    Segmentor                *segmentor;
    cv::Mat                  originalMat;
    cv::Mat                  overlayMat;
};

#endif // IMGOPERATORWINDOWS_H
