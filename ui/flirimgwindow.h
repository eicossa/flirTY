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
#include <QProgressBar>
#include <QComboBox>

class flirImgWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void openImagesFileDialog();
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
    flirImgWindow(QMainWindow *parent = 0);
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
    void setupProgressBars();
    void setupPaletteComboBox();

    flirImg      *fimg;
    Segmentor    *segmentor;
    flirImgFrame *mainImgFrame;

    QSplitter    *v1Splitter;
    QSplitter    *v2Splitter;
    QGraphicsScene *scene;
    QComboBox    *paletteComboBox;

    QGraphicsPixmapItem originalPixmap, processedPixmap;
    //cv::Mat originalMat, processedMat;
    QImage originalImage, processedImage;

    QProgressBar *pBarMetadata;
    QProgressBar *pBarTempdata;
    QProgressBar *pBarSegmentr;

    QToolBar     *mainMenuToolbar;

    QAction      *infernoAction;
};


#endif // FLIRBABAWINDOW_H
