#include "segmentorwindow.h"
#include "segmentor.h"
#include "segmentorframe.h"

#include <opencv2/highgui/highgui.hpp>

#include <QFileDialog>
#include <QDir>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDebug>
#include <QDockWidget>
#include <QLabel>
#include <QListWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLCDNumber>

segmentorWindow::segmentorWindow(QWidget *parent) :
    QWidget(parent)
{
    // qDebug() << "segmentorWindow : segmentorWindow() -- Constructor called";
}

void segmentorWindow::initSegmentorWindow(Segmentor *s)
{
    this->segmentor  = s;
    updateSegmentorObjDetails();

    createAllImgFrames();
    createBlobSliderDock();
    // qDebug() << "segmentorWindow : readImage() .. done with createBlobSliderDock()";
    createMorphologyDock();
    // qDebug() << "segmentorWindow : readImage() .. done with createMorphologyDock()";
    connectEverythingUp();
    // qDebug() << "segmentorWindow : readImage() .. done with connections";
    updateRangeSliderAfterSegmentorObjectHasChanged();

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(origImgFrame);
    hlayout->addWidget(finalImgFrame);
    hlayout->addWidget(selectedAreasImgFrame);

    QGridLayout *glayout = new QGridLayout;
    glayout->addWidget(((QWidget*) morphologyDock),     0, 0);
    glayout->addWidget(((QWidget*) blobAreaSliderDock), 0, 1);


    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addItem(hlayout);
    vlayout->addItem(glayout);

    setLayout(vlayout);

    setWindowTitle(tr("Img Segmentor"));
}

void segmentorWindow::createAllImgFrames()
{
    // qDebug() << "segmentorWindow -- createAllImgFrames() -- begun";
    origImgFrame     = new segmentorFrame("Orig",               segmentor->getOriginalMat());
//    filteredImgFrame = new segmentorFrame("Filtered",           segmentor->getFilteredMat());
//    morphologizedImgFrame = new segmentorFrame("Morphologized", segmentor->getMorphologizedMat());

//    binaryImgFrame   = new segmentorFrame("Binary",             segmentor->getBinaryMat());
//    distanceImgFrame = new segmentorFrame("Distance",           segmentor->getDistTransformedMat());
//    peaksImgFrame    = new segmentorFrame("Peaks",              segmentor->getPeaksMat());
//    markersImgFrame  = new segmentorFrame("Markers",            segmentor->getMarkersMat());
    finalImgFrame         = new segmentorFrame("Final",    segmentor->getSegmentedColoredMat());
    selectedAreasImgFrame = new segmentorFrame("Selected", segmentor->getOverlayMat());
    // qDebug() << "segmentorWindow -- createAllImgFrames() -- ended";
}

segmentorWindow::~segmentorWindow()
{
    //delete ui;
}

cv::Mat segmentorWindow::getOverlayImage()
{
    //return segmentor->getFinalMat();
    return segmentor->getOverlayMat();
}

void segmentorWindow::morphologyOperator(int op)
{
    //qDebug()<<"Morphology Operator : "<<op;
    segmentor->params()->setMorphologyOperator(op);
    segmentor->segmentImage();
    this->updateSegmentorObjDetails();
    this->updateAllFrames();
    this->updateRangeSliderAfterSegmentorObjectHasChanged();
}

void segmentorWindow::morphologyKernelShape(int shp)
{
    //qDebug()<<"Morphology Shape : "<<shp;
    segmentor->params()->setMorphologKernelShape(shp);
    segmentor->segmentImage();
    this->updateSegmentorObjDetails();
    this->updateAllFrames();
    this->updateRangeSliderAfterSegmentorObjectHasChanged();
}

void segmentorWindow::morphologyKernelSize(int size)
{
    //qDebug()<<"Kernel Size : "<<size;
    segmentor->params()->setMorphologyKernelSize(size);
    segmentor->segmentImage();
    this->updateSegmentorObjDetails();
    this->updateAllFrames();
    this->updateRangeSliderAfterSegmentorObjectHasChanged();
}


void segmentorWindow::morphologyIternum(int size)
{
    //qDebug()<<"Kernel Size : "<<size;
    segmentor->params()->setMorphologyIternum(size);
    segmentor->segmentImage();
    this->updateSegmentorObjDetails();
    this->updateAllFrames();
    this->updateRangeSliderAfterSegmentorObjectHasChanged();
}

void segmentorWindow::setupMorphologicalKernelOperatorGroupBox()
{
    // QGroupBox *operatorGroupBox;
    // QLayout *vbox1;

    operatorGroupBox     = new QGroupBox(tr("Operator"));
    QRadioButton *radio1 = new QRadioButton(tr("Opening"));
    QRadioButton *radio2 = new QRadioButton(tr("Closing"));
    QRadioButton *radio3 = new QRadioButton(tr("Gradient"));
    QRadioButton *radio4 = new QRadioButton(tr("TopHat"));
    QRadioButton *radio5 = new QRadioButton(tr("BlackHat"));
    radio3->setChecked(true);
    operatorButtonGroup  = new QButtonGroup(this);
    operatorButtonGroup->addButton(radio1, 0);
    operatorButtonGroup->addButton(radio2, 1);
    operatorButtonGroup->addButton(radio3, 2);
    operatorButtonGroup->addButton(radio4, 3);
    operatorButtonGroup->addButton(radio5, 4);
    //operatorButtonGroup->checkedId();


    vbox1 = new QVBoxLayout;
    //vbox1->addWidget(operatorButtonGroup);
    vbox1->addWidget(radio1);
    vbox1->addWidget(radio2);
    vbox1->addWidget(radio3);
    vbox1->addWidget(radio4);
    vbox1->addWidget(radio5);
    vbox1->setSpacing(1);

    operatorGroupBox->setLayout(vbox1);
}


void segmentorWindow::setupMorphologicalKernelShapeGroupBox()
{
    // QGroupBox *kernelGroupBox;
    // QLayout *vbox2;

    kernelGroupBox = new QGroupBox(tr("Kernel"));
    QRadioButton *radio11 = new QRadioButton(tr("Rectangle"));
    QRadioButton *radio21 = new QRadioButton(tr("Circle"));
    QRadioButton *radio31 = new QRadioButton(tr("Ellipse"));
    radio31->setChecked(true);
    kernelButtonGroup = new QButtonGroup(this);
    kernelButtonGroup->addButton(radio11, 0);
    kernelButtonGroup->addButton(radio21, 1);
    kernelButtonGroup->addButton(radio31, 2);

    vbox2 = new QVBoxLayout;
    vbox2->addWidget(radio11);
    vbox2->addWidget(radio21);
    vbox2->addWidget(radio31);
    vbox2->setSpacing(1);

    kernelGroupBox->setLayout(vbox2);
}


void segmentorWindow::setupMorphologicalKernelSizeSliders()
{
    // QGroupBox *sizeSlidersGroupBox;
    // QSlider *kernelSizeSlider;
    // QSlider *iternumSizeSlider;
    // QLayout *sliderboxlayout;

    sizeSlidersGroupBox = new QGroupBox(tr("Size Sliders"));

    kernelSizeSlider = new QSlider(Qt::Horizontal);
    kernelSizeSlider->setMinimum(0);
    kernelSizeSlider->setMaximum(21);
    kernelSizeSlider->setTickInterval(1);
    kernelSizeSlider->setValue(3);

    iternumSizeSlider = new QSlider(Qt::Horizontal);
    iternumSizeSlider->setMinimum(1);
    iternumSizeSlider->setMaximum(100);
    iternumSizeSlider->setTickInterval(1);
    iternumSizeSlider->setValue(1);

    sliderboxlayout = new QVBoxLayout;
    sliderboxlayout->addWidget(kernelSizeSlider);
    sliderboxlayout->addWidget(iternumSizeSlider);
    sliderboxlayout->setSpacing(1);

    sizeSlidersGroupBox->setLayout(sliderboxlayout);
}

void segmentorWindow::createMorphologyDock()
{

    morphologyDock = new QDockWidget(tr("Morphology"), this);
    morphologyDock->setAllowedAreas(Qt::LeftDockWidgetArea);

    QFrame *settingsFrame = new QFrame(morphologyDock);

    setupMorphologicalKernelOperatorGroupBox();
    setupMorphologicalKernelShapeGroupBox();
    setupMorphologicalKernelSizeSliders();

    QLayout* hboxlayout = new QHBoxLayout;
    hboxlayout->addWidget(operatorGroupBox);
    hboxlayout->addWidget(kernelGroupBox);
    hboxlayout->addWidget(sizeSlidersGroupBox);

    settingsFrame->setLayout(hboxlayout);
    settingsFrame->setMinimumWidth(300);
    settingsFrame->setMaximumWidth(600);

    morphologyDock->setWidget(settingsFrame);
}



void segmentorWindow::setupBlobAreaSlidersGroupBox()
{
    // qDebug() << " segmentorWindow -- setupBlobAreaSlidersGroupBox() -- start";
    blobSizeSlidersGroupBox = new QGroupBox(tr("Blob Area Sliders"));

    blobSizeSliderLayout = new QVBoxLayout;

    minimumSizeSliderLabel = new QLabel("min Blob Area", this);
    minBlobAreaDisplay     = new QLCDNumber();
    minBlobAreaDisplay->setMinimumWidth(150);
    minBlobAreaDisplay->setMinimumHeight(100);
    maximumSizeSliderLabel = new QLabel("max Blob Area", this);
    maxBlobAreaDisplay     = new QLCDNumber();
    maxBlobAreaDisplay->setMinimumWidth(150);
    maxBlobAreaDisplay->setMinimumHeight(100);
    minBlobAreaDisplay->setDigitCount(7);
    maxBlobAreaDisplay->setDigitCount(7);

    rS                     = new RangeSlider(this);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(minimumSizeSliderLabel);
    hLayout->addWidget(minBlobAreaDisplay);
    hLayout->addStretch();
    hLayout->addWidget(maxBlobAreaDisplay);
    hLayout->addWidget(maximumSizeSliderLabel);

    blobSizeSliderLayout->addItem(hLayout);
    blobSizeSliderLayout->addWidget(rS);
    blobSizeSliderLayout->addStretch();

    blobSizeSlidersGroupBox->setLayout(blobSizeSliderLayout);

    // qDebug() << " segmentorWindow -- setupBlobAreaSlidersGroupBox() -- end";
}

void segmentorWindow::updateRangeSliderAfterSegmentorObjectHasChanged()
{
    // qDebug() << " segmentorWindow : updateBlobAreaSliders() -- begun ";
    emit  displayMinBlobArea(sortedBlobAreasElemAtPos(0));
    emit  displayMaxBlobArea(sortedBlobAreasElemAtPos(numOfBlobs-1));
    rS->SetRange(0, numOfBlobs-1);
    // qDebug() << " segmentorWindow : updateBlobAreaSliders() -- finis ";
}



int segmentorWindow::sortedBlobAreasElemAtPos(int pos)
{

    // qDebug() << " segmentorWindow : sortedBlobAreasElemAtPos() -- begun "
    //          << pos << "/" << numOfBlobs;
    if(pos < 0 || pos >=(numOfBlobs)){
        // qDebug() << " segmentorWindow : sortedBlobAreasElemAtPos() -- Da faq man " << pos;
        // std::exit(1);
        return -1;
    }
    else {
        // qDebug() << " segmentorWindow : sortedBlobAreasElemAtPos() "
        //          << " -- About to retrieve " << pos;
        return segmentor->getSortedBlobAreas().at(pos);
    }
}


void segmentorWindow::updateSegmentorObjDetails()
{
    numOfBlobs = segmentor->getBlobAreas().size();
    currMinBlobAreaPosition = 0;
    currMaxBlobAreaPosition = numOfBlobs - 1;
    blobSmallestArea = segmentor->getMinArea();
    blobLargestArea  = segmentor->getMaxArea();
    //segmentor->generateSelectedBlobAreas(currMinBlobAreaPosition, currMaxBlobAreaPosition);
    segmentor->updateWithSelectedBlobAreas(currMinBlobAreaPosition, currMaxBlobAreaPosition);
}

void segmentorWindow::updateSegmentorObjSelectedAreaDetails()
{
    //segmentor->generateSelectedBlobAreas(currMinBlobAreaPosition, currMaxBlobAreaPosition);
    segmentor->updateWithSelectedBlobAreas(currMinBlobAreaPosition, currMaxBlobAreaPosition);
}

void segmentorWindow::printCurrentlySelectedAreas()
{
    updateSegmentorObjSelectedAreaDetails();
    updateSelectedAreasFrame();

}

void segmentorWindow::displayMinBlobAreaSliderKaValue(int minv)
{
    currMinBlobAreaPosition = minv;
    emit displayMinBlobArea(sortedBlobAreasElemAtPos(minv));
    printCurrentlySelectedAreas();
}

void segmentorWindow::displayMaxBlobAreaSliderKaValue(int maxv)
{
    currMaxBlobAreaPosition = maxv;
    emit displayMaxBlobArea(sortedBlobAreasElemAtPos(maxv));
    printCurrentlySelectedAreas();
}

void segmentorWindow::connectEverythingUp()
{
    connect(operatorButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(morphologyOperator(int)));
    connect(kernelButtonGroup,   SIGNAL(buttonClicked(int)), this, SLOT(morphologyKernelShape(int)));
    connect(kernelSizeSlider,    SIGNAL(valueChanged(int)),  this, SLOT(morphologyKernelSize(int)));
    connect(iternumSizeSlider,   SIGNAL(valueChanged(int)),  this, SLOT(morphologyIternum(int)));

    connect(rS, SIGNAL(lowerValueChanged(int)), this, SLOT(displayMinBlobAreaSliderKaValue(int)));
    connect(rS, SIGNAL(upperValueChanged(int)), this, SLOT(displayMaxBlobAreaSliderKaValue(int)));

    connect(this, SIGNAL(displayMinBlobArea(int)), minBlobAreaDisplay, SLOT(display(int)));
    connect(this, SIGNAL(displayMaxBlobArea(int)), maxBlobAreaDisplay, SLOT(display(int)));
}


#include "rangeslider.h"
void segmentorWindow::createBlobSliderDock()
{
    // qDebug() << " segmentorWindow : createBlobSliderDock() -- begun";
    setupBlobAreaSlidersGroupBox();

    blobAreaSliderDock = new QDockWidget(tr("Blob Area"), this);
    blobAreaSliderDock->setAllowedAreas(Qt::RightDockWidgetArea);
    QFrame *settings2Frame = new QFrame(blobAreaSliderDock);
    QLayout *hboxLayout    = new QHBoxLayout;
    hboxLayout->addWidget(blobSizeSlidersGroupBox);

    settings2Frame->setLayout(hboxLayout);
    settings2Frame->setMinimumWidth(300);
    settings2Frame->setMaximumWidth(600);

    blobAreaSliderDock->setWidget(settings2Frame);
    // qDebug() << " segmentorWindow : createBlobSliderDock() -- ended";
}

void segmentorWindow::createBinaryThresholdingDock()
{
    binaryThresholdDock    = new QDockWidget(tr("Binary Threhold"), this);
    QFrame *settings3Frame = new QFrame(binaryThresholdDock);
    QLayout *hboxLayout    = new QHBoxLayout;
    hboxLayout->addWidget(binaryThresholdGroupBox);

    settings3Frame->setLayout(hboxLayout);
    settings3Frame->setMinimumWidth(300);
    settings3Frame->setMaximumWidth(600);

    blobAreaSliderDock->setWidget(settings3Frame);
}

void segmentorWindow::createDistanceTransformDock()
{
    distanceTransformDock = new QDockWidget(tr("Distance Transform"), this);
}

void segmentorWindow::updateSelectedAreasFrame()
{
    if(selectedAreasImgFrame){
        selectedAreasImgFrame->updateProcessedImage(segmentor->getOverlayMat());
    }
}

void segmentorWindow::updateAllFrames()
{

    // qDebug()<<"-----------------------------------------------------";
    origImgFrame->updateProcessedImage(segmentor->getOriginalMat());
    //filteredImgFrame->updateProcessedImage(segmentor->getFilteredMat());
    //morphologizedImgFrame->updateProcessedImage(segmentor->getMorphologizedMat());

    //binaryImgFrame->updateProcessedImage(segmentor->getBinaryMat());
    //distanceImgFrame->updateProcessedImage(segmentor->getDistTransformedMat());
    //peaksImgFrame->updateProcessedImage(segmentor->getPeaksMat());

    //markersImgFrame->updateProcessedImage(segmentor->getMarkersMat());
    finalImgFrame->updateProcessedImage(segmentor->getSegmentedColoredMat());
    selectedAreasImgFrame->updateProcessedImage(segmentor->getOverlayMat());

    // qDebug() << " segmentorWindow -- updateAllFrames() -- About to update blob area sliders ";
    // updateBlobAreaSliders();
    // qDebug() << " segmentorWindow -- updateAllFrames() -- Done with updating blob area sliders";
}

void segmentorWindow::fullUpdate()
{
    updateAllFrames();
    updateRangeSliderAfterSegmentorObjectHasChanged();
}
