#include "tgfstabbedwindow.h"
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QMenuBar>
#include "tgfsmainwindow.h"
#include <QThread>


void oneImageAtATime(tgfsTabbedWindow &w)
{
    flirbabaWindow *fW = new flirbabaWindow();
    segmentorWindow *sW = new segmentorWindow();

    w.addView(fW, QString("Flirbaba Viewer"));
    w.addView(sW, QString("Segmentor"));


    w.loadDefaultImage();
    w.connectEverythingForOneImage();
}

void multipleImagesAtATime(tgfsTabbedWindow &w)
{
    w.load5Images();
    w.connectEverythingFor5Images();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create tabbed views
    tgfsTabbedWindow w;
    w.setupMenubars();
    oneImageAtATime(w);
    //multipleImagesAtATime(w);

    w.showMaximized();

    return a.exec();
}
