#include "tgfstabbedwindow.h"
#include "tgfstabbedwindowprivate.h"
#include "imgoperatorwindows.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>

tgfsTabbedWindow::tgfsTabbedWindow(QWidget *parent) :
    QMainWindow(parent),
    d_ptr(new tgfsTabbedWindowPrivate(this))
{

    numOfImagesDisplayed = 0;

    // Add test menubar
    QAction *openFileDialogAction = new QAction("Open", this);
    openFileDialogAction->setStatusTip(tr("Open Radiometric JPEG"));
    //menubarWindow->setupImgReader(openFileDialogAction);
    fileMenu = this->menuBar()->addMenu("File");
    fileMenu->addAction(openFileDialogAction);

    editMenu = this->menuBar()->addMenu("Edit");
    editMenu->addAction(openFileDialogAction);

    helpMenu = this->menuBar()->addMenu("Help");
    helpMenu->addAction(openFileDialogAction);

    connect(openFileDialogAction, SIGNAL(triggered(bool)),
            this,                 SLOT(openImagesFileDialog()));
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


void tgfsTabbedWindow::addAnotherTab(QString imgPath)
{
    flirImg *fimg = new flirImg(imgPath);
    flirImgWindow *fW;
    fW = new flirImgWindow();
    fW->initFlirImgWindow(fimg);
    addView(fW, QString(fimg->getFileName()));
    numOfImagesDisplayed += 1;
}

void tgfsTabbedWindow::loadDefaultImage()
{
    qDebug() << "tgfsTabbedWindow::Current Path : " << QDir::currentPath();
    QString currPath = QDir::currentPath();
    QString imgPath1  = currPath + "/nabtdimgs/FLIR1097/FLIR1097.jpg";
    QString imgPath2  = currPath + "/nabtdimgs/FLIR0179/FLIR0179.jpg";
//    QString imgPath3  = currPath + "/nabtdimgs/FLIR1329/FLIR1311.jpg";
//    QString imgPath4  = currPath + "/nabtdimgs/FLIR1329/FLIR1312.jpg";
//    QString imgPath5  = currPath + "/nabtdimgs/FLIR1329/FLIR1319.jpg";
//    QString imgPath6  = currPath + "/nabtdimgs/FLIR1329/FLIR1320.jpg";
//    QString imgPath7  = currPath + "/nabtdimgs/FLIR1329/FLIR1321.jpg";
//    QString imgPath8  = currPath + "/nabtdimgs/FLIR1329/FLIR1322.jpg";
    QString imgPath9  = currPath + "/nabtdimgs/FLIR1329/FLIR1329.jpg";


    addAnotherTab(imgPath1);
    addAnotherTab(imgPath2);
//    addAnotherTab(imgPath3);
//    addAnotherTab(imgPath4);
//    addAnotherTab(imgPath5);
//    addAnotherTab(imgPath6);
//    addAnotherTab(imgPath7);
//    addAnotherTab(imgPath8);
    addAnotherTab(imgPath9);
}


void tgfsTabbedWindow::openImagesFileDialog()
{
  //custom behavior
  //QString fileName = QFileDialog::getOpenFileName(/*args*/);

  QString imgPath = QFileDialog::getOpenFileName(this,
                                          tr("Open Input Image"),
                                          QDir::homePath(),
                                          tr("Images") + " (*.jpg *.png *.bmp)");

  addAnotherTab(imgPath);
}
