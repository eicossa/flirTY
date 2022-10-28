#include "tabbedwindow.h"
#include "tgfstabbedwindowprivate.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>

tabbedWindow::tabbedWindow(QWidget *parent) :
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

    //editMenu = this->menuBar()->addMenu("Edit");
    //editMenu->addAction(openFileDialogAction);

    //helpMenu = this->menuBar()->addMenu("Help");
    //helpMenu->addAction(openFileDialogAction);

    connect(openFileDialogAction, SIGNAL(triggered(bool)),
            this,                 SLOT(openImagesFileDialog()));
}

tabbedWindow::~tabbedWindow()
{
    delete d_ptr;
}


QWidget* tabbedWindow::currentView()
{
    return d_ptr->currentView();
}

void tabbedWindow::setCurrentView(int index)
{
    d_ptr->setCurrentView(index);
}

int tabbedWindow::insertView(const QPoint &pos,
                                 QWidget *page,
                                 const QString &text)
{
    return d_ptr->insertView(pos, page, text);
}

int tabbedWindow::addView(QWidget *view, const QString &title)
{
    return d_ptr->addView(view, title);
}

void tabbedWindow::removeView(int index)
{
    d_ptr->removeView(index);
}

#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QTimer>
#include <QToolButton>


void tabbedWindow::addAnotherTab(QString imgPath)
{
    flirImg *fimg = new flirImg(imgPath);
    flirImgWindow *fW;
    fW = new flirImgWindow();
    fW->initFlirImgWindow(fimg);
    addView(fW, QString(fimg->getFileName()));
    numOfImagesDisplayed += 1;
}

void tabbedWindow::openImagesFileDialog()
{
  //custom behavior
  //QString fileName = QFileDialog::getOpenFileName(/*args*/);

  QString imgPath = QFileDialog::getOpenFileName(this,
                                          tr("Open Input Image"),
                                          QDir::homePath(),
                                          tr("Images") + " (*.jpg *.png *.bmp)");

  addAnotherTab(imgPath);
}
