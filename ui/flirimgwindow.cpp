#include "flirimgwindow.h"
//#include "flirimgpixel.h"
#include "flirimgwindow.h"
#include "flirimgframe.h"
#include "flirimg.h"
#include "segmentor.h"

#include <QMessageBox>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>

using namespace std;

flirImgWindow::flirImgWindow(QMainWindow *parent)
    : QMainWindow(parent)
{
    // qDebug() << "flirbabaWindow : flirBabaWindow() constructor ";
}

#include <QFileDialog>
void flirImgWindow::initFlirImgWindow(flirImg* flirImgObject)
{
    this->fimg      = flirImgObject;
    this->segmentor = flirImgObject->getSegmentorObject();

    mainImgFrame     = new flirImgFrame("main display", fimg);

    // qDebug() << " flirbabaWindow::readImage() -- created a new imgFrame";
    UiSetupWindow();
    createDockWindows();
    connectEverything();
}

#include <QDockWidget>
void flirImgWindow::createDockWindows()
{
    createMorphologyDock();
    createBlobSliderDock();
    addDockWidget(Qt::RightDockWidgetArea, morphologyDock);
    addDockWidget(Qt::RightDockWidgetArea, blobAreaSliderDock);
    morphologyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    blobAreaSliderDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    viewMenu->addAction(morphologyDock->toggleViewAction());
    viewMenu->addAction(blobAreaSliderDock->toggleViewAction());
}



void flirImgWindow::connectEverything()
{


    connect(kernelSizeSlider,    SIGNAL(valueChanged(int)),  this, SLOT(morphologyKernelSize(int)));
    connect(iternumSizeSlider,   SIGNAL(valueChanged(int)),  this, SLOT(morphologyIternum(int)));

    //connect(blobAreaRSlider, SIGNAL(lowerValueChanged(int)), this, SLOT(displayMinBlobAreaSliderKaValue(int)));
    //connect(blobAreaRSlider, SIGNAL(upperValueChanged(int)), this, SLOT(displayMaxBlobAreaSliderKaValue(int)));

    connect(mainImgFrame->transparencySlider, SIGNAL(valueChanged(int)),
            mainImgFrame,                     SLOT(updateOverlayTransparency(int)));
    //connect()

    // connect(this, SIGNAL(displayMinBlobArea(int)), minBlobAreaDisplay, SLOT(display(int)));
    // connect(this, SIGNAL(displayMaxBlobArea(int)), maxBlobAreaDisplay, SLOT(display(int)));


}

void flirImgWindow::UiSetupWindow()
{
    v1Splitter = new QSplitter;
    v2Splitter = new QSplitter;

    QSplitter *hSplitter = new QSplitter;
    hSplitter->setOrientation(Qt::Horizontal);
    hSplitter->addWidget(v1Splitter);
    hSplitter->addWidget(v2Splitter);

    v1Splitter->addWidget(mainImgFrame);

    // Added test toolbar
    flirImgToolbar = this->addToolBar("flirImgToolbar");
    //tb->addAction("Instructions");
    viewMenu = this->menuBar()->addMenu("View");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(hSplitter);

    setupProgressBars();
    setupPaletteComboBox();

    QWidget* w = new QWidget();
    w->setLayout(layout);
    setCentralWidget(w);

    setWindowTitle(tr("flirTY"));
}

void flirImgWindow::setupScene()
{

}

//cv::COLORMAP_AUTUMN = 0,
//cv::COLORMAP_BONE = 1,
//cv::COLORMAP_JET = 2,
//cv::COLORMAP_WINTER = 3,
//cv::COLORMAP_RAINBOW = 4,
//cv::COLORMAP_OCEAN = 5,
//cv::COLORMAP_SUMMER = 6,
//cv::COLORMAP_SPRING = 7,
//cv::COLORMAP_COOL = 8,
//cv::COLORMAP_HSV = 9,
//cv::COLORMAP_PINK = 10,
//cv::COLORMAP_HOT = 11,
//cv::COLORMAP_PARULA = 12,
//cv::COLORMAP_MAGMA = 13,
//cv::COLORMAP_INFERNO = 14,
//cv::COLORMAP_PLASMA = 15,
//cv::COLORMAP_VIRIDIS = 16,
//cv::COLORMAP_CIVIDIS = 17,
//cv::COLORMAP_TWILIGHT = 18,
//cv::COLORMAP_TWILIGHT_SHIFTED = 19
#include <QComboBox>
void flirImgWindow::setupPaletteComboBox()
{
    paletteComboBox = new QComboBox();
    paletteComboBox->addItem("autumn");
    paletteComboBox->addItem("bone");
    paletteComboBox->addItem("jet");
    paletteComboBox->addItem("winter");
    paletteComboBox->addItem("rainbow");
    paletteComboBox->addItem("ocean");
    paletteComboBox->addItem("summer");
    paletteComboBox->addItem("spring");
    paletteComboBox->addItem("cool");
    paletteComboBox->addItem("hsv");
    paletteComboBox->addItem("pink");
    paletteComboBox->addItem("hot");
    paletteComboBox->addItem("parula");
    paletteComboBox->addItem("magma");
    paletteComboBox->addItem("inferno");
    paletteComboBox->addItem("plasma");
    paletteComboBox->addItem("viridis");
    paletteComboBox->addItem("cividis");
    paletteComboBox->addItem("twilight");
    paletteComboBox->addItem("twilight_s");

    connect(paletteComboBox, SIGNAL(currentIndexChanged(int)),
            mainImgFrame,    SLOT(changePalette(int)));

    flirImgToolbar->addWidget(paletteComboBox);
}

void flirImgWindow::setupProgressBars()
{
    //pBarFrame = new progressBarFrame();

    pBarMetadata = new QProgressBar();
    pBarMetadata->setMinimum(0);
    pBarMetadata->setMaximum(100);
    pBarMetadata->setVisible(true);
    pBarMetadata->setEnabled(true);

    flirImgToolbar->addWidget(pBarMetadata);

    setupMorphologicalKernelOperatorGroupBox();
    setupMorphologicalKernelShapeGroupBox();
    setupMorphologicalKernelSizeSliders();
}
