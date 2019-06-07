#include "tgfsmainwindow.h"
#include "progressbarframe.h"

#include <QAction>
#include <QPushButton>
#include <QDebug>
#include <QProgressBar>

tgfsMainWindow::tgfsMainWindow(QMainWindow *parent) :
    QMainWindow(parent)
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
    //layout->addWidget(pBar2);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    setWindowTitle(tr("Img Segmentor"));

}


void tgfsMainWindow::refreshProgressBars()
{
    pBarMetadata->setValue(0);
    pBarTempdata->setValue(0);
    pBarSegmentr->setValue(0);
}
