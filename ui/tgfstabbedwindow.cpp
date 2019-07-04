#include "tgfstabbedwindow.h"
#include "tgfstabbedwindowprivate.h"
#include "imgoperatorwindows.h"

tgfsTabbedWindow::tgfsTabbedWindow(QWidget *parent) :
    QMainWindow(parent),
    d_ptr(new tgfsTabbedWindowPrivate(this))
{
    menubarWindow = new tgfsMainWindow();


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

void tgfsTabbedWindow::openImagesFileDialog()
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
  loadUi();
}

void tgfsTabbedWindow::loadUi()
{
    menubarWindow->refreshProgressBars();

    removeView(2);
    removeView(1);

    fW->close();
    sW->close();

    fW = new flirImgWindow();
    sW = new segmentorWindow();

    fW->initFlirImgWindow(fimg, segmentor);
    sW->initSegmentorWindow(segmentor);


    addView(fW, QString("Flirbaba Viewer"));

    addView(sW, QString("Segmentor"));

    setCurrentView(1);
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
    connect(fimg,                                    SIGNAL(signalFlirImgProgress(int)),
            menubarWindow->getTempdataProgressBar(), SLOT(setValue(int)));
//    connect(segmentor,                               SIGNAL(signalSegmentorProgress(int)),
//            menubarWindow->getSegmentrProgressBar(), SLOT(setValue(int)));

    // connect(fimg,                                    SIGNAL(signalFlirImgProgress(int)),
    //         this,                                    SLOT(aSignalHasBeenReceived(int)));
    // connect(segmentor,                               SIGNAL(signalSegmentorProgress(int)),
    //         this,                                    SLOT(aSignalHasBeenReceived(int)));
}
