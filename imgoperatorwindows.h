#ifndef IMGOPERATORWINDOWS_H
#define IMGOPERATORWINDOWS_H

#include "segmentor.h"
#include "flirimg.h"
<<<<<<< HEAD
#include "ui/segmentorwindow.h"
#include "ui/flirimgwindow.h"
=======
#include "segmentorwindow.h"
#include "flirbabawindow.h"
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

class imgOperatorWindows : public QObject
{
    Q_OBJECT
public slots:
<<<<<<< HEAD
    void            isPointKeBlobKaTempNikaaloMadarchod(QPoint p);
=======
    void            isPointKeBlobKaTempNikaaloMadarchod(QPointF p);
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
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
<<<<<<< HEAD
    flirImgWindow*  getFlirbabaWindow()  { return this->fW; }
=======
    flirbabaWindow*  getFlirbabaWindow()  { return this->fW; }
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
    segmentorWindow* getSegmentorWindow() { return this->sW; }

private:
    int                      numOfBlobs;
<<<<<<< HEAD
    flirImgWindow           *fW;
=======
    flirbabaWindow           *fW;
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
    segmentorWindow          *sW;
    flirImg                  *fimg;
    Segmentor                *segmentor;
    cv::Mat                  originalMat;
    cv::Mat                  overlayMat;
};

#endif // IMGOPERATORWINDOWS_H
