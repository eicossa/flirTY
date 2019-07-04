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

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
//#include "cvflirbabainterface.h"
#include "flirimg.h"
#include "flirimgframe.h"
#include "segmentor.h"

class QGraphicsScene;
class QSplitter;
class flirImg;

#include <QMainWindow>

class flirImgWindow : public QWidget
{
    Q_OBJECT
public slots:
//    void emitLeafButtonSignal(bool t)
//    { qDebug() << " flirbabaWindow::emitLeafButtonSignal() - leaf button is pressed ka signal emitted ";
//      emit leafDetectButtonToggled(t); }
//    void emitBlobBataaoIsPointKaSignal(QPoint t)
//    { qDebug() << " flirbabaWindow::emitBlobBataaoIsPointKaSignal - blob bataao is point ka signal emitted ";
//      emit plssBlobBataaoIsPointKa(t); }
//    void emitBlobAvgTempMilaBhosadiwaale(double d)
//    { qDebug() << " flirbabaWindow::emitBlobAvgTempMilaBhosadiwaale(d) - blob avg temp mil gaya signal emitted " << d;
//      emit blobAvgTempLeleMadarchod(d); }
public:
    flirImgWindow(QWidget *parent = 0);
    void initFlirImgWindow(flirImg*, Segmentor*);
    void setupScene();
    void updateOverlaidImage(cv::Mat);
    void restoreOriginalImage();
signals:
//    void leafDetectButtonToggled(bool t);
//    void plssBlobBataaoIsPointKa(QPoint t);
//    void blobAvgTempLeleMadarchod(double d);
private:
    void UiSetupWindow();
    void setupMatrix();
    void convert16bitGrayscaleToQImage();
    void connectEverything();

    flirImg      *fimg;
    Segmentor    *segmentor;
    flirImgFrame *mainImgFrame;

    QSplitter    *v1Splitter;
    QSplitter    *v2Splitter;
    QGraphicsScene *scene;

    QGraphicsPixmapItem originalPixmap, processedPixmap;
    //cv::Mat originalMat, processedMat;
    QImage originalImage, processedImage;
};


#endif // FLIRBABAWINDOW_H
