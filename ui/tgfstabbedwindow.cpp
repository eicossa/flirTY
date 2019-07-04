#include "tgfstabbedwindow.h"
#include "tgfstabbedwindowprivate.h"
#include "imgoperatorwindows.h"

tgfsTabbedWindow::tgfsTabbedWindow(QWidget *parent) :
    QMainWindow(parent),
    d_ptr(new tgfsTabbedWindowPrivate(this))
{
    fW            = new flirImgWindow();
    sW            = new segmentorWindow();
    fimg          = new flirImg();
    segmentor     = new Segmentor();
    numOfImagesDisplayed = 1;

    setupMenubars();
}

tgfsTabbedWindow::~tgfsTabbedWindow()
{
    delete d_ptr;
}


QWidget* tgfsTabbedWindow::currentView()
{
    return d_ptr->currentView();
}

void tgfsTabbedWindow::setCurrentView(int index)
{
    d_ptr->setCurrentView(index);
}

int tgfsTabbedWindow::insertView(const QPoint &pos,
                                 QWidget *page,
                                 const QString &text)
{
    return d_ptr->insertView(pos, page, text);
}

int tgfsTabbedWindow::addView(QWidget *view, const QString &title)
{
    return d_ptr->addView(view, title);
}

void tgfsTabbedWindow::removeView(int index)
{
    d_ptr->removeView(index);
}

#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QTimer>
#include <QToolButton>
void tgfsTabbedWindow::setupMenubars()
{}



void tgfsTabbedWindow::loadUi()
{
    menubarWindow->refreshProgressBars();

    removeView(1);
    removeView(0);

    fW->close();
    sW->close();

    fW = new flirImgWindow();
    sW = new segmentorWindow();

    fW->initFlirImgWindow(fimg, segmentor);
    sW->initSegmentorWindow(segmentor);

    addView(fW, QString("Flirbaba Viewer"));
    addView(sW, QString("Segmentor"));

    setCurrentView(0);
}

void tgfsTabbedWindow::loadDefaultImage()
{
    qDebug() << "tgfsTabbedWindow::Current Path : " << QDir::currentPath();
    QString currPath = QDir::currentPath();
    //QString imgPath  = currPath + "/nabtdimgs/FLIR1097/FLIR1097.jpg";
    QString imgPath  = currPath + "/nabtdimgs/FLIR1329/FLIR1329.jpg";

    fimg->processImage(imgPath.toStdString());
    segmentor = fimg->getSegmentorObject();

    loadUi();
    connectEverythingForOneImage();
}

void tgfsTabbedWindow::connectEverythingForOneImage()
{
//    connect(segmentor,                               SIGNAL(signalSegmentorProgress(int)),
//            menubarWindow->getSegmentrProgressBar(), SLOT(setValue(int)));

    // connect(fimg,                                    SIGNAL(signalFlirImgProgress(int)),
    //         this,                                    SLOT(aSignalHasBeenReceived(int)));
    // connect(segmentor,                               SIGNAL(signalSegmentorProgress(int)),
    //         this,                                    SLOT(aSignalHasBeenReceived(int)));
}
