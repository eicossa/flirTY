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
void flirImgWindow::initFlirImgWindow(flirImg* flirImgObject, Segmentor* segmentorObject)
{
    this->fimg      = flirImgObject;
    this->segmentor = segmentorObject;

    mainImgFrame     = new flirImgFrame("Left view", fimg, segmentor);

    // qDebug() << " flirbabaWindow::readImage() -- created a new imgFrame";
    UiSetupWindow();
    connectEverything();
}

void flirImgWindow::openImagesFileDialog()
{
  //custom behavior
  //QString fileName = QFileDialog::getOpenFileName(/*args*/);

  QString imgPath = QFileDialog::getOpenFileName(this,
                                          tr("Open Input Image"),
                                          QDir::homePath(),
                                          tr("Images") + " (*.jpg *.png *.bmp)");
  // need something here
  // tu idhar aayega saale, jab deliver karna hoga
  fimg->processImage(imgPath.toStdString());
  segmentor = fimg->getSegmentorObject();
}

void flirImgWindow::connectEverything()
{

    //    connect(mainImgFrame, SIGNAL(leafButtonStatus(bool)),
    //            this,         SLOT(emitLeafButtonSignal(bool)));
    //    connect(mainImgFrame, SIGNAL(blobIDBataaoIsPointKa(QPoint)),
    //            this,         SLOT(emitBlobIDBataaoIsPointKaSignal(QPoint)));


    //    connect(this,    SIGNAL(blobAvgTempLeleMadarchod(double)),
    //            mainImgFrame, SLOT(updateBlobAvgTemp(double)));
    //    connect(mainImgFrame, SIGNAL(blobAvgTempBataaoIsPointKa(QPoint)),
    //            this,    SLOT(emitBlobBataaoIsPointKaSignal(QPoint)));
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
    mainMenuToolbar = this->addToolBar("toolbar");
    //tb->addAction("Instructions");

    // Add test menubar
    QAction *openFileDialogAction = new QAction("Open", this);
    openFileDialogAction->setStatusTip(tr("Open Radiometric JPEG"));
    //menubarWindow->setupImgReader(openFileDialogAction);
    QMenu *fileMenu = this->menuBar()->addMenu("File");
    fileMenu->addAction(openFileDialogAction);

    QMenu *editMenu = this->menuBar()->addMenu("Edit");
    editMenu->addAction(openFileDialogAction);

    QMenu *helpMenu = this->menuBar()->addMenu("Help");
    helpMenu->addAction(openFileDialogAction);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(hSplitter);



    connect(openFileDialogAction, SIGNAL(triggered(bool)),
            this,                 SLOT(openImagesFileDialog()));
    // setLayout(layout);
    //QLayout* l = (QHBoxLayout*)(this->layout());
    //l->addWidget(mainImgFrame);
    //setLayout(l);

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

    mainMenuToolbar->addWidget(paletteComboBox);
}

void flirImgWindow::setupProgressBars()
{
    //pBarFrame = new progressBarFrame();

    pBarMetadata = new QProgressBar();
    pBarMetadata->setMinimum(0);
    pBarMetadata->setMaximum(100);
    pBarMetadata->setVisible(true);
    pBarMetadata->setEnabled(true);

    pBarTempdata = new QProgressBar();
    pBarTempdata->setMinimum(0);
    pBarTempdata->setMaximum(100);
    pBarTempdata->setVisible(true);
    pBarTempdata->setEnabled(true);

    pBarSegmentr = new QProgressBar();
    pBarSegmentr->setMinimum(0);
    pBarSegmentr->setMaximum(100);
    pBarSegmentr->setVisible(true);
    pBarSegmentr->setEnabled(true);
//    QProgressBar *pBar2 = new QProgressBar();
//    pBar2->setMinimum(0);
//    pBar2->setMaximum(100);
//    pBar2->setVisible(true);
//    pBar2->setEnabled(true);

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(pBarMetadata);
    layout->addWidget(pBarTempdata);
    layout->addWidget(pBarSegmentr);


    mainMenuToolbar->addWidget(pBarMetadata);
    //mainMenuToolbar->addWidget(pBarTempdata);
    //mainMenuToolbar->addWidget(pBarSegmentr);

}

//void flirImgWindow::updateOverlaidImage(cv::Mat overlay)
//{
//    mainImgFrame->displayOverlaidImage(overlay);
//}

//void flirImgWindow::restoreOriginalImage()
//{
//    mainImgFrame->displayOriginalImage();
//}
