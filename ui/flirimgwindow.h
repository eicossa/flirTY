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
#include "rangeslider.h"

class QGraphicsScene;
class QSplitter;
class flirImg;

#include <QMainWindow>
#include <QProgressBar>
#include <QComboBox>
#include <QTextEdit>
#include <QListWidget>
#include <QListWidget>
#include <QGroupBox>

class flirImgWindow : public QMainWindow
{
    Q_OBJECT

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
    void initFlirImgWindow(flirImg*);
    void setupScene();
    void updateOverlaidImage(cv::Mat);
    void restoreOriginalImage();
    void createDockWindows();
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

    QToolBar     *flirImgToolbar;

    QAction      *infernoAction;

    QTextEdit    *textEdit;
    QListWidget  *customerList;
    QListWidget  *paragraphsList;


    QMenu*        viewMenu;


    QDockWidget    *morphologyDock;
    QDockWidget    *blobAreaSliderDock;
    QDockWidget    *binaryThresholdDock;
    QDockWidget    *distanceTransformDock;

    QButtonGroup   *operatorButtonGroup, *kernelButtonGroup;

    void           initSegmentorWindow();
    void           createMorphologyDock();
    void           createBlobSliderDock();
    //void           createBinaryThresholdingDock();
    //void           createDistanceTransformDock();

    void           updateOverlay();
    void           updateSelectedAreasFrame();
    void           fullUpdate();
    void           updateSegmentorObjDetails();
    void           updateSegmentorObjSelectedAreaDetails();

    int            sortedBlobAreasElemAtPos(int);
    void           printCurrentlySelectedAreas();

    void           connectEverythingUp();

    void           setupMorphologicalKernelOperatorGroupBox();
    void           setupMorphologicalKernelShapeGroupBox();
    void           setupMorphologicalKernelSizeSliders();
    void           setupBlobAreaSlidersGroupBox();
    void           updateRangeSliderAfterSegmentorObjectHasChanged();

    // binary threshold groupbox
    QGroupBox      *binaryThresholdGroupBox;
    QSlider        *binaryLoThreshSlider;
    QSlider        *binaryHiThreshSlider;


    // setup morphological kernel size sliders
    QGroupBox      *sizeSlidersGroupBox;
    QSlider        *kernelSizeSlider;
    QSlider        *iternumSizeSlider;
    QLayout        *sliderboxlayout;


    // setup morphological kernel shape groupbox
    QGroupBox      *kernelGroupBox;
    QLayout        *vbox2;

    // setup morphological kernel operator groupbox
    QGroupBox      *operatorGroupBox;
    QLayout        *vbox1;

    // setup blob area size sliders
    int            currMinBlobAreaPosition;
    int            currMaxBlobAreaPosition;
    int            numOfBlobs;
    int            blobSmallestArea;
    int            blobLargestArea;

    QGroupBox      *blobSizeSlidersGroupBox;
    QSlider        *blobMinimumSizeSlider;
    QSlider        *blobMaximumSizeSlider;
    QVBoxLayout    *blobSizeSliderLayout;
    QLabel         *minimumSizeSliderLabel;
    QLabel         *maximumSizeSliderLabel;
    QLCDNumber     *minBlobAreaDisplay;
    QLCDNumber     *maxBlobAreaDisplay;

    RangeSlider    *rS;


signals:
    void           displayMinBlobArea(int);
    void           displayMaxBlobArea(int);
private slots:
    void           morphologyOperator(int);
    void           morphologyKernelShape(int);
    void           morphologyKernelSize(int);
    void           morphologyIternum(int);

    // void           displayMinBlobAreaSliderKaValue(int);
    // void           displayMaxBlobAreaSliderKaValue(int);
};


#endif // FLIRBABAWINDOW_H
