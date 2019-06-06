#ifndef SEGMENTORWINDOW_H
#define SEGMENTORWINDOW_H

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "segmentor.h"
#include "segmentorframe.h"
#include "rangeslider.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLCDNumber>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class segmentorWindow : public QWidget
{
    Q_OBJECT
signals:
    void           displayMinBlobArea(int);
    void           displayMaxBlobArea(int);
private slots:
    void           morphologyOperator(int);
    void           morphologyKernelShape(int);
    void           morphologyKernelSize(int);
    void           morphologyIternum(int);

    void           displayMinBlobAreaSliderKaValue(int);
    void           displayMaxBlobAreaSliderKaValue(int);
//    void           dontLetMinBlobAreaSliderCrossMaxBlobAreaSlider(int);
//    void           dontLetMaxBlobAreaSliderCrossMinBlobAreaSlider(int);
public:
    explicit       segmentorWindow(QWidget *parent = 0);
    void           readImage(Segmentor* segmentor);
    ~segmentorWindow();
    cv::Mat        getOverlayImage();
    void           createAllImgFrames();
private:
    Segmentor      *segmentor;

    segmentorFrame *origImgFrame;
    segmentorFrame *filteredImgFrame;
    segmentorFrame *morphologizedImgFrame;

    segmentorFrame *binaryImgFrame;
    segmentorFrame *distanceImgFrame;
    segmentorFrame *peaksImgFrame;

    segmentorFrame *markersImgFrame;
    segmentorFrame *finalImgFrame;
    segmentorFrame *selectedAreasImgFrame;

    QDockWidget    *morphologyDock;
    QDockWidget    *blobAreaSliderDock;
    QDockWidget    *binaryThresholdDock;
    QDockWidget    *distanceTransformDock;

    QButtonGroup   *operatorButtonGroup, *kernelButtonGroup;

    void           readImage();
    void           createMorphologyDock();
    void           createBlobSliderDock();
    void           createBinaryThresholdingDock();
    void           createDistanceTransformDock();

    void           updateAllFrames();
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
    QVBoxLayout        *blobSizeSliderLayout;
    QLabel         *minimumSizeSliderLabel;
    QLabel         *maximumSizeSliderLabel;
    QLCDNumber     *minBlobAreaDisplay;
    QLCDNumber     *maxBlobAreaDisplay;

    RangeSlider    *rS;
};

#endif // SEGMENTORWINDOW_H
