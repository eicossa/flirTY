#include "flirimgwindow.h"
//#include "flirimgpixel.h"
#include "flirimgwindow.h"
#include "flirimgframe.h"
#include "flirimg.h"
#include "segmentor.h"

#include <QMessageBox>

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
    setupWindow();
    connectEverything();
}

void flirImgWindow::connectEverything()
{
    connect(mainImgFrame, SIGNAL(leafButtonStatus(bool)),
            this,         SLOT(emitLeafButtonSignal(bool)));
//    connect(mainImgFrame, SIGNAL(blobIDBataaoIsPointKa(QPoint)),
//            this,         SLOT(emitBlobIDBataaoIsPointKaSignal(QPoint)));


//    connect(this,    SIGNAL(blobAvgTempLeleMadarchod(double)),
//            mainImgFrame, SLOT(updateBlobAvgTemp(double)));
//    connect(mainImgFrame, SIGNAL(blobAvgTempBataaoIsPointKa(QPoint)),
//            this,    SLOT(emitBlobBataaoIsPointKaSignal(QPoint)));
}

void flirImgWindow::setupWindow()
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

    setWindowTitle(tr("FLIR Img Processor"));
}

void flirImgWindow::setupScene()
{

}

void flirImgWindow::updateOverlaidImage(cv::Mat overlay)
{
    mainImgFrame->updateWithOverlay(overlay);
}

void flirImgWindow::restoreOriginalImage()
{
    mainImgFrame->restoreOriginalImage();
}
