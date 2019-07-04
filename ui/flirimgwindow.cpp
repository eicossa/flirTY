#include "flirimgwindow.h"
//#include "flirimgpixel.h"
#include "flirimgwindow.h"
#include "flirimgframe.h"
#include "flirimg.h"
#include "segmentor.h"

#include <QMessageBox>
#include <QToolBar>

using namespace std;

flirImgWindow::flirImgWindow(QWidget *parent)
    : QWidget(parent)
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

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(hSplitter);

    setLayout(layout);
//    QLayout* l = this->layout();
//    l->addWidget(hSplitter);

    setWindowTitle(tr("FLIR Img Processor"));

    //addView(this, QString("Base"));
    // Added test toolbar
//    QToolBar *tb = this->addToolBar("toolbar");
//    tb->addAction("Instructions");



//    // Add test menubar
//    QAction *openFileDialogAction = new QAction("Open IR Image", menubarWindow);
//    //menubarWindow->setupImgReader(openFileDialogAction);
//    QMenu *m = this->menuBar()->addMenu("File");
//    m->addAction(openFileDialogAction);
    // newAct = new QAction(tr("&New"), this);
    // newAct->setShortcuts(QKeySequence::New);
    // newAct->setStatusTip(tr("Create a new file"));
    // connect(openFileDialogAction, &QAction::triggered, this, &flirbabaWindow::openImagesFileDialog);
//    connect(openFileDialogAction, SIGNAL(triggered(bool)),
//            this, SLOT(openImagesFileDialog()));
}

void flirImgWindow::setupScene()
{

}

//void flirImgWindow::updateOverlaidImage(cv::Mat overlay)
//{
//    mainImgFrame->displayOverlaidImage(overlay);
//}

//void flirImgWindow::restoreOriginalImage()
//{
//    mainImgFrame->displayOriginalImage();
//}
