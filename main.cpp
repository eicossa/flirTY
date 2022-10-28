
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QMenuBar>
#include <QThread>
#include <QDebug>

#include "ui/management/tabbedwindow.h"


void testOnlyFlirImg()
{
    #include <flirimg/flirimg.h>
    QString currPath = QDir::currentPath();
    QString imgPath  = currPath + "/nabtdimgs/FLIR1097.jpg";
    flirImg *fimg;
    fimg = new flirImg();
    fimg->processImage(imgPath.toStdString());
    qDebug() << "testOnlyFlirImg -- Done";
}


void load_tabs(tabbedWindow &w)
{
    QString currPath = QDir::currentPath();
    QString imgPath1 = "/home/eicossa/Data/nabtd/new/20160512_011655_303_R.JPG";
    w.addAnotherTab(imgPath1);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // when only need to debug the flirImg shizz
    // testOnlyFlirImg();

    // Create tabbed views
    tabbedWindow w;
    load_tabs(w);
    w.showMaximized();

    return a.exec();
}
