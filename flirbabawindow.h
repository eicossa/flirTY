#ifndef FLIRBABAWINDOW_H
#define FLIRBABAWINDOW_H

#include <QWidget>
#include <QDir>
#include <QDebug>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFileDialog>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include "cvflirbabainterface.h"
#include "flirimg.h"
#include "flirimgframe.h"

class QGraphicsScene;
class QSplitter;
class flirImg;

class flirbabaWindow : public QWidget
{
    Q_OBJECT
public slots:
    void emitLeafButtonSignal(bool t)
    { qDebug() << " flirbabaWindow::emitLeafButtonSignal() - leaf button is pressed ka signal emitted ";
      emit leafDetectButtonToggled(t); }
    void emitBlobBataaoIsPointKaSignal(QPointF t)
    { qDebug() << " flirbabaWindow::emitBlobBataaoIsPointKaSignal - blob bataao is point ka signal emitted ";
      emit plssBlobBataaoIsPointKa(t); }
    void emitBlobAvgTempMilaBhosadiwaale(double d)
    { qDebug() << " flirbabaWindow::emitBlobAvgTempMilaBhosadiwaale(d) - blob avg temp mil gaya signal emitted " << d;
      emit blobAvgTempLeleMadarchod(d); }
public:
    flirbabaWindow(QWidget *parent = 0);
    void readImage(flirImg*);
    void setupScene();
    void updateOverlaidImage(cv::Mat);
    void restoreOriginalImage();
signals:
    void leafDetectButtonToggled(bool t);
    void plssBlobBataaoIsPointKa(QPointF t);
    void blobAvgTempLeleMadarchod(double d);
private:
    void setupWindow();
    void setupMatrix();
    void convert16bitGrayscaleToQImage();
    void connectEverything();

    flirImg *fimg;
    flirImgFrame *mainImg;

    QSplitter *v1Splitter;
    QSplitter *v2Splitter;
    QGraphicsScene *scene;

    QGraphicsPixmapItem originalPixmap, processedPixmap;
    //cv::Mat originalMat, processedMat;
    QImage originalImage, processedImage;
};


#endif // FLIRBABAWINDOW_H
