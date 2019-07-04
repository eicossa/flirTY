#include "flirimgframe.h"
#include <QToolButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QLabel>
#include <QButtonGroup>

//class QLabel;
//class QSlider;
//class QToolButton;
//class flirImgFrame;
//class QHBoxLayout;
//class QVBoxLayout;
//class QStatusBar;
//class flirImg;
//class QRubberBand;

void flirImgFrame::UiSetupZoomStuff()
{
    zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/chips-images/zoomin.png"));
    zoomInIcon->setIconSize(*iconSize);

    zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/chips-images/zoomout.png"));
    zoomOutIcon->setIconSize(*iconSize);

    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    transparencySlider = new QSlider;
    transparencySlider->setMinimum(0);
    transparencySlider->setMaximum(10);
    transparencySlider->setValue(5);
    transparencySlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(transparencySlider);
    zoomSliderLayout->addWidget(zoomOutIcon);
}

void flirImgFrame::UiSetupRotateStuff()
{
    rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/chips-images/rotateleft.png"));
    rotateLeftIcon->setIconSize(*iconSize);

    rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/chips-images/rotateright.png"));
    rotateRightIcon->setIconSize(*iconSize);

    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);
}

void flirImgFrame::UiSetupTopButtonsLayout()
{
    // Label layout
    topButtonsLayout  = new QHBoxLayout;
    imgDisplayModeLbl = new QLabel(tr("Image Display Mode"));

    // labelLayout->addWidget(label);
    // labelLayout->addStretch();
    topButtonsLayout->addWidget(imgDisplayModeLbl);
    topButtonsLayout->addWidget(rawImageViewButton);
    topButtonsLayout->addWidget(origImageViewButton);
    topButtonsLayout->addWidget(repalettedImageViewButton);
    topButtonsLayout->addStretch();
    topButtonsLayout->addWidget(pointSelectButton);
    topButtonsLayout->addWidget(leafDetectButton);
    // topButtonsLayout->addWidget(rectSelectButton);
    // topButtonsLayout->addWidget(printButton);
}


#include <QToolBar>
void flirImgFrame::UiLayoutEverything()
{
    UiSetupTopButtonsLayout();

    resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);

    statusBar = new QStatusBar;
    statusBar->setStatusTip(tr("This is the status bar"));

    lcdDisplay1Label = new QLabel();
    lcdDisplay1 = new QLCDNumber();
    lcdDisplay1->setDigitCount(6);
    lcdDisplay1->setMinimumWidth(300);

    lcdDisplay2Label = new QLabel();
    lcdDisplay2 = new QLCDNumber();
    lcdDisplay2->setDigitCount(6);
    lcdDisplay2->setMinimumWidth(300);

    QVBoxLayout *tempDisplaysLayout = new QVBoxLayout;
    tempDisplaysLayout->addWidget(lcdDisplay1Label);
    tempDisplaysLayout->addWidget(lcdDisplay1);
    tempDisplaysLayout->addWidget(lcdDisplay2Label);
    tempDisplaysLayout->addWidget(lcdDisplay2);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(topButtonsLayout, 0, 0);
    topLayout->addWidget(flirImgDisplayer, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addLayout(tempDisplaysLayout, 1, 2);
    topLayout->addLayout(rotateSliderLayout, 2, 0);
    topLayout->addWidget(resetButton, 2, 1);
    topLayout->addWidget(statusBar, 3, 0);

    setLayout(topLayout);



}

void flirImgFrame::UiSetupImgDisplayer()
{
    flirImgDisplayer = new flirImgDisplayerGraphicsView(this);
    flirImgDisplayer->setRenderHint(QPainter::Antialiasing, false);
    flirImgDisplayer->setDragMode(QGraphicsView::RubberBandDrag);
    flirImgDisplayer->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    flirImgDisplayer->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    flirImgDisplayer->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    flirImgDisplayer->setMouseTracking(true); //to be able to retrieve image pixel coords
}

void flirImgFrame::UiSetupViewingModes()
{
    rawImageViewButton = new QToolButton;
    rawImageViewButton->setText(tr("Raw"));
    rawImageViewButton->setCheckable(true);
    rawImageViewButton->setChecked(true);

    origImageViewButton = new QToolButton;
    origImageViewButton->setText(tr("Original"));
    origImageViewButton->setCheckable(true);
    origImageViewButton->setChecked(false);

    repalettedImageViewButton = new QToolButton;
    repalettedImageViewButton->setText(tr("Repaletted"));
    repalettedImageViewButton->setCheckable(true);
    repalettedImageViewButton->setChecked(false);

    QButtonGroup *pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(rawImageViewButton);
    pointerModeGroup->addButton(origImageViewButton);
    pointerModeGroup->addButton(repalettedImageViewButton);
}

void flirImgFrame::UiSetupProcessingModes()
{
    leafDetectButton = new QToolButton;
    leafDetectButton->setText(tr("LeafDetect"));
    leafDetectButton->setCheckable(true);
    leafDetectButton->setChecked(false);

    // rectSelectButton = new QToolButton;
    // rectSelectButton->setText(tr("RectSelect"));
    // rectSelectButton->setCheckable(true);
    // rectSelectButton->setChecked(false);


    pointSelectButton = new QToolButton;
    pointSelectButton->setText(tr("PointSelect"));
    pointSelectButton->setCheckable(true);
    pointSelectButton->setChecked(true);

    // printButton = new QToolButton;
    // printButton->setIcon(QIcon(QPixmap(":/chips-images/fileprint.png")));

    QButtonGroup *processingModeGroup = new QButtonGroup(this);
    processingModeGroup->setExclusive(true);
    processingModeGroup->addButton(leafDetectButton);
    //processingModeGroup->addButton(rectSelectButton);
    processingModeGroup->addButton(pointSelectButton);
}
