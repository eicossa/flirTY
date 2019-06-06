#include "flirbabawindow.h"
//#include "flirimgpixel.h"
#include "flirbabawindow.h"
#include "flirimgframe.h"
#include "flirimg.h"

#include <QMessageBox>

using namespace std;

flirbabaWindow::flirbabaWindow(QWidget *parent)
    : QWidget(parent)
{
    // qDebug() << "flirbabaWindow : flirBabaWindow() constructor ";
}

#include <QFileDialog>
void flirbabaWindow::readImage(flirImg* flirImgObject)
{
    this->fimg  = flirImgObject;

    mainImg     = new flirImgFrame("Left view", fimg);

    // qDebug() << " flirbabaWindow::readImage() -- created a new imgFrame";
    setupWindow();
    connectEverything();
}

void flirbabaWindow::connectEverything()
{
    connect(mainImg, SIGNAL(leafButtonStatus(bool)),
            this,    SLOT(emitLeafButtonSignal(bool)));

    connect(mainImg, SIGNAL(blobAvgTempBataaoIsPointKa(QPoint)),
            this,    SLOT(emitBlobBataaoIsPointKaSignal(QPoint)));
    connect(mainImg, SIGNAL(blobIDBataaoIsPointKa(QPoint)),
            this,    SLOT(emitBlobIDBataaoIsPointKaSignal(QPoint)));


    connect(this,    SIGNAL(blobAvgTempLeleMadarchod(double)),
            mainImg, SLOT(updateBlobAvgTemp(double)));
}

void flirbabaWindow::setupWindow()
{
    v1Splitter = new QSplitter;
    v2Splitter = new QSplitter;

    QSplitter *hSplitter = new QSplitter;
    hSplitter->setOrientation(Qt::Horizontal);
    hSplitter->addWidget(v1Splitter);
    hSplitter->addWidget(v2Splitter);

    v1Splitter->addWidget(mainImg);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(hSplitter);

    setLayout(layout);

    setWindowTitle(tr("FLIR Img Processor"));
}

void flirbabaWindow::setupScene()
{

}

void flirbabaWindow::updateOverlaidImage(cv::Mat overlay)
{
    mainImg->updateWithOverlay(overlay);
}

void flirbabaWindow::restoreOriginalImage()
{
    mainImg->restoreOriginalImage();
}
