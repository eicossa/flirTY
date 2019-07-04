#include "ui/tgfstabbedwindow.h"
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QMenuBar>
#include "ui/tgfsmainwindow.h"
#include <QThread>

void testOnlyFlirImg()
{
    #include <flirimg.h>
    QString currPath = QDir::currentPath();
    QString imgPath  = currPath + "/nabtdimgs/FLIR1097/FLIR1097.jpg";
    flirImg *fimg;
    fimg = new flirImg();
    fimg->processImage(imgPath.toStdString());
    qDebug() << "testOnlyFlirImg -- Done";
}


void oneImageAtATime(tgfsTabbedWindow &w)
{
    flirImgWindow *fW = new flirImgWindow();
    segmentorWindow *sW = new segmentorWindow();

    w.addView(fW, QString("F"));
    w.addView(sW, QString("S"));


    w.loadDefaultImage();
    w.connectEverythingForOneImage();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // when only need to debug the flirImg shizz
    //testOnlyFlirImg();

    // Create tabbed views
    tgfsTabbedWindow w;
    oneImageAtATime(w);
    w.showMaximized();

    return a.exec();
}
