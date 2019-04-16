#include "tgfstabbedwindow.h"
#include "tgfstabbedwindowprivate.h"
#include "imgoperatorwindows.h"

tgfsTabbedWindow::tgfsTabbedWindow(QWidget *parent) :
    QMainWindow(parent),
    d_ptr(new tgfsTabbedWindowPrivate(this))
{
    menubarWindow = new tgfsMainWindow();


    fW            = new flirbabaWindow();
    sW            = new segmentorWindow();
    fimg          = new flirImg();
    segmentor     = new Segmentor();
    numOfImagesDisplayed = 1;


    fWindows      = std::vector<flirbabaWindow*>();
    sWindows      = std::vector<segmentorWindow*>();
    fImgs         = std::vector<flirImg*>();
    segmentors    = std::vector<Segmentor*>();
    for(int loopi=0; loopi <5; loopi++){
        fWindows.push_back(new flirbabaWindow());
        sWindows.push_back(new segmentorWindow());
        fImgs.push_back(new flirImg());
        segmentors.push_back(new Segmentor());
    }
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

int tgfsTabbedWindow::insertView(const QPoint &pos, QWidget *page,
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
{
    //tgfsMainWindow mW;
    //QMainWindow mW;
    //mW.setStyleSheet("QMainWindow { background-color: red; }");
    addView(menubarWindow, QString("Base"));

    // Added test toolbar
    QToolBar *tb = menubarWindow->addToolBar("toolbar");
    tb->addAction("Instructions");

    // Add test menubar
    QAction *openFileDialogAction = new QAction("Open IR Image", menubarWindow);
    //menubarWindow->setupImgReader(openFileDialogAction);
    QMenu *m = menubarWindow->menuBar()->addMenu("File");
    m->addAction(openFileDialogAction);
    // newAct = new QAction(tr("&New"), this);
    // newAct->setShortcuts(QKeySequence::New);
    // newAct->setStatusTip(tr("Create a new file"));
    // connect(openFileDialogAction, &QAction::triggered, this, &flirbabaWindow::openImagesFileDialog);

    connect(openFileDialogAction, SIGNAL(triggered(bool)),
            this, SLOT(openImagesFileDialog()));



    //QTimer::singleShot(2000, this, SLOT(loadDefaultImage()));
}

void tgfsTabbedWindow::isPointKeBlobKaTempNikaaloMadarchod(QPointF p)
{
    // qDebug() << " tgfsTabbedWindow::isPointKeBlobKaTempNikaaloMadarchod(QPointF p) - Inside madarchod blob nikaalo " << p.x() << ", " << p.y();
    std::vector<Point2i> pts;
    pts = segmentor->getIsPointKaBlob(p);
    std::cout << " Num of points in this blob " << pts.size() << std::endl;
    double avgBlobTemp;
    avgBlobTemp = fimg->calcBlobAvgTemp(pts);
    emit blobAvgTempLeleBhosdike(avgBlobTemp);
    // std::cout << "Avg blob Temp " << avgBlobTemp << endl;
    // std::cout << "Blob num " << blobNum << std::endl;
}


void tgfsTabbedWindow::aSignalHasBeenReceived(int p)
{
    // cout << "Received signal with value " << p << endl;
}

void tgfsTabbedWindow::openImagesFileDialog()
{
  //custom behavior
  //QString fileName = QFileDialog::getOpenFileName(/*args*/);

  QString fileName = QFileDialog::getOpenFileName(this,
                                          tr("Open Input Image"),
                                          QDir::homePath(),
                                          tr("Images") + " (*.jpg *.png *.bmp)");
  makeEverythingHappenWithOneImageAtATime(fileName);
}


















void tgfsTabbedWindow::loadDefaultImage()
{
    qDebug() << "Current Path : " << QDir::currentPath();
    QString currPath = QDir::currentPath();
    QString imgPath  = currPath + "/nabtdimgs/FLIR1097.jpg";

     makeEverythingHappenWithOneImageAtATime(imgPath);
    // makeEverythingHappenANewWay(imgPath);
}

void tgfsTabbedWindow::makeEverythingHappenANewWay(QString imgPath)
{
    imgOperatorWindows* iOW;

    iOW = new imgOperatorWindows();
    iOW->buildShitForThisImgpath(imgPath);
}

void tgfsTabbedWindow::makeEverythingHappenWithOneImageAtATime(QString imgPath)
{
    menubarWindow->refreshProgressBars();

    removeView(2);
    removeView(1);

    fW->close();
    sW->close();

    buildFlirImgObjAndSegmentorObj(imgPath);

    fW = new flirbabaWindow();
    sW = new segmentorWindow();

    fW->readImage(fimg);
    sW->readImage(segmentor);

    addView(fW, QString("Flirbaba Viewer"));
    addView(sW, QString("Segmentor"));

    setCurrentView(1);

    connectEverythingForOneImage();
}

void tgfsTabbedWindow::buildFlirImgObjAndSegmentorObj(QString imgPath)
{
    fimg->loadOrigImage(imgPath.toStdString());
    fimg->loadRAWThermalImage();

    flirImgMetadata *fmd;
    fmd = fimg->getMetadata();
    // fmd->printReadMetadata();
    // fmd->printCalcedMetadata();

    if(fimg->didThermalConversionSucceed()){
        // fimg->printImageSummary();
        // cout<<"Began processing..." << endl;
        fimg->calcTempForEveryPixel();
        // cout<<"End of processing..." << endl;
    }
    else {
        qDebug() << "Thermal conversion failed";
        std::exit(1);
    }

    qDebug() << " tgfsTabbedWindow  : buildSegmentorObj() - Beginning with the segmentor object ";
    std::string thermimgpath = fimg->getThermalImgPath();
    qDebug() << " tgfsTabbedWindow  : buildSegmentorObj() - About to build new segmentor object ";
    segmentor->loadThermalImage(thermimgpath, cv::IMREAD_COLOR);
    qDebug() << " tgfsTabbed Window : buildSegmentorObj() - About to segment it ";
    segmentor->segmentImage();
    qDebug() << " tgfsTabbed Window : buildSegmentorObj() - Done ";
}



void tgfsTabbedWindow::connectEverythingForOneImage()
{

    connect(fimg->getMetadata(),                     SIGNAL(signalFlirImgMetadataProgress(int)),
            menubarWindow->getMetadataProgressBar(), SLOT(setValue(int)));
    connect(fimg,                                    SIGNAL(signalFlirImgProgress(int)),
            menubarWindow->getTempdataProgressBar(), SLOT(setValue(int)));
    connect(segmentor,                               SIGNAL(signalSegmentorProgress(int)),
            menubarWindow->getSegmentrProgressBar(), SLOT(setValue(int)));

    connect(fimg->getMetadata(),                     SIGNAL(signalFlirImgMetadataProgress(int)),
            this,                                    SLOT(aSignalHasBeenReceived(int)));
    connect(fimg,                                    SIGNAL(signalFlirImgProgress(int)),
            this,                                    SLOT(aSignalHasBeenReceived(int)));
    connect(segmentor,                               SIGNAL(signalSegmentorProgress(int)),
            this,                                    SLOT(aSignalHasBeenReceived(int)));

    connect(fW,                                      SIGNAL(leafDetectButtonToggled(bool)),
            this,                                    SLOT(displayOverlaidImageForOneImage(bool)));
    connect(fW,                                      SIGNAL(plssBlobBataaoIsPointKa(QPointF)),
            this,                                    SLOT(isPointKeBlobKaTempNikaaloMadarchod(QPointF)));
    connect(this,                                    SIGNAL(blobAvgTempLeleBhosdike(double)),
            fW,                                      SLOT(emitBlobAvgTempMilaBhosadiwaale(double)));
}

void tgfsTabbedWindow::displayOverlaidImageForOneImage(bool y)
{
    cv::Mat overlayMat = sW->getOverlayImage();

    qDebug() << " tgfsTabbedWindow : displayOverlaidImageForOneImage() - inside the decider";
    if(y){
      qDebug() << " tgfsTabbedWindow : displayOverlaidImageForOneImage() - true overlaid image ";
      fW->updateOverlaidImage(overlayMat);
    }
    else {
      qDebug() << " tgfsTabbedWindow : displayOverlaidImageForOneImage() - restore original image";
      //cout << " false overlaid image " << endl;
      fW->restoreOriginalImage();
    }
}







void tgfsTabbedWindow::load5Images()
{
    qDebug() << "Current Path : " << QDir::currentPath();
    QString currPath = QDir::currentPath();
    QString imgPath  = currPath + "/nabtdimgs/FLIR1097.jpg";

    makeEverythingHappenWith5ImagesAtATime(imgPath);
}

void tgfsTabbedWindow::addAnotherFlirImgAndSegmentorObj(QString imgPath)
{
    numOfImagesDisplayed+=1;

    flirImg   *f = fImgs.at(numOfImagesDisplayed);
    Segmentor *s = segmentors.at(numOfImagesDisplayed);

    f->loadOrigImage(imgPath.toStdString());
    f->loadRAWThermalImage();
    s->loadThermalImage(f->getThermalImgPath(), cv::IMREAD_COLOR);
    s->segmentImage();

    flirImgMetadata *fmd;
    fmd = f->getMetadata();
    if(f->didThermalConversionSucceed()){
        f->calcTempForEveryPixel();
    } else{
        qDebug() << " Thermal Conversion failed ";
        std::exit(1);
    }

    flirbabaWindow *fw  = fWindows.at(numOfImagesDisplayed);
    segmentorWindow *sw = sWindows.at(numOfImagesDisplayed);

    fw->readImage(f);
    sw->readImage(s);

    addView(fw, QString("fviewer"));
    addView(sw, QString("sviewer"));

    connect(fw, SIGNAL(leafDetectButtonToggled(bool)),
            this, SLOT(displayOverlaidImageForOneImage(bool)));
    connect(fw, SIGNAL(plssBlobBataaoIsPointKa(QPointF)),
            this, SLOT(isPointKeBlobKaTempNikaaloMadarchod(QPointF)));
    connect(this, SIGNAL(blobAvgTempLeleBhosdike(double)),
            fw, SLOT(emitBlobAvgTempMilaBhosadiwaale(double)));
}

void tgfsTabbedWindow::makeEverythingHappenWith5ImagesAtATime(QString imgPath)
{
    menubarWindow->refreshProgressBars();

    addAnotherFlirImgAndSegmentorObj(imgPath);
    addAnotherFlirImgAndSegmentorObj(imgPath);
    addAnotherFlirImgAndSegmentorObj(imgPath);
}

void tgfsTabbedWindow::connectEverythingFor5Images()
{
//    for(int loopi=0; loopi < numOfImagesDisplayed; loopi++){
//        flirImg          *f = fImgs.at(loopi);
//        Segmentor        *s = segmentors.at(loopi);
//        flirbabaWindow  *fw = fWindows.at(loopi);
//        segmentorWindow *sw = sWindows.at(loopi);

//        connect(f->getMetadata(), SIGNAL(signalFlirImgMetadataProgress(int)),
//                menubarWindow->getMetadataProgressBar(), SLOT(setValue(int)));
//        connect(f, SIGNAL(signalFlirImgProgress(int)),
//                menubarWindow->getTempdataProgressBar(), SLOT(setValue(int)));
//        connect(segmentor, SIGNAL(signalSegmentorProgress(int)),
//                menubarWindow->getSegmentrProgressBar(), SLOT(setValue(int)));
//        connect(f->getMetadata(), SIGNAL(signalFlirImgMetadataProgress(int)),
//                this, SLOT(aSignalHasBeenReceived(int)));
//        connect(f, SIGNAL(signalFlirImgProgress(int)),
//                this, SLOT(aSignalHasBeenReceived(int)));
//        connect(s, SIGNAL(signalSegmentorProgress(int)),
//                this, SLOT(aSignalHasBeenReceived(int)));
//        connect(fw, SIGNAL(leafDetectButtonToggled(bool)),
//                this, SLOT(displayOverlaidImage(bool)));
//        connect(fw, SIGNAL(plssBlobBataaoIsPointKa(QPointF)),
//                this, SLOT(isPointKeBlobKaTempNikaaloMadarchod(QPointF)));
//        connect(this, SIGNAL(blobAvgTempLeleBhosdike(double)),
//                fw, SLOT(emitBlobAvgTempMilaBhosadiwaale(double)));
//    }

//    flirImg          *f;
//    Segmentor        *s;
//    flirbabaWindow  *fw;
//    segmentorWindow *sw;

//    f  = fImgs.at(0);
//    s  = segmentors.at(0);
//    fw = fWindows.at(0);
//    sw = sWindows.at(0);


//    connect(f->getMetadata(), SIGNAL(signalFlirImgMetadataProgress(int)),
//            menubarWindow->getMetadataProgressBar(), SLOT(setValue(int)));
//    connect(f, SIGNAL(signalFlirImgProgress(int)),
//            menubarWindow->getTempdataProgressBar(), SLOT(setValue(int)));
//    connect(segmentor, SIGNAL(signalSegmentorProgress(int)),
//            menubarWindow->getSegmentrProgressBar(), SLOT(setValue(int)));
//    connect(f->getMetadata(), SIGNAL(signalFlirImgMetadataProgress(int)),
//            this, SLOT(aSignalHasBeenReceived(int)));
//    connect(f, SIGNAL(signalFlirImgProgress(int)),
//            this, SLOT(aSignalHasBeenReceived(int)));
//    connect(s, SIGNAL(signalSegmentorProgress(int)),
//            this, SLOT(aSignalHasBeenReceived(int)));
//    connect(fw,   SIGNAL(leafDetectButtonToggled(bool)),
//            this, SLOT(displayOverlaidImageFor5ImagesAtATime(bool)));
//    connect(fw,   SIGNAL(plssBlobBataaoIsPointKa(QPointF)),
//            this, SLOT(isPointKeBlobKaTempNikaaloMadarchod(QPointF)));
//    connect(this, SIGNAL(blobAvgTempLeleBhosdike(double)),
//            fw,   SLOT(emitBlobAvgTempMilaBhosadiwaale(double)));

}


void tgfsTabbedWindow::displayOverlaidImageFor5ImagesAtATime(bool y)
{
    qDebug() << " tgfsTabbedWindow : displayOverlaidImageFor5ImagesAtATime() : we in here man";
    //cv::Mat overlayMat = sW->getOverlayImage();

    if(y){
      //cout << " true overlaid image " << endl;
      //fw->updateOverlaidImage(sw->getOverlayImage());
    }
    else {
      //cout << " false overlaid image " << endl;
      //fw->restoreOriginalImage();
    }
}
