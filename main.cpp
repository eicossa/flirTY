<<<<<<< HEAD
#include "ui/tgfstabbedwindow.h"
=======
#include "tgfstabbedwindow.h"
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QMenuBar>
<<<<<<< HEAD
#include "ui/tgfsmainwindow.h"
#include <QThread>

//void changeEndianness(unsigned short x)
//{
//    unsigned short x1, x2, x3, y;
//    x1 = x >> 8;
//    x2 = x & 0xff;
//    x3 = x2 << 8;
//    y  = x1 + x3;
//    qDebug() << " x, x1, x3, y " << x << x1 << x3 << y;
//}

//void testOnlyFlirImg()
//{
//    #include <flirimg.h>
//    QString currPath = QDir::currentPath();
//    QString imgPath  = currPath + "/nabtdimgs/FLIR1097.jpg";
//    flirImg *fimg;
//    fimg = new flirImg();
//    fimg->processImage(imgPath.toStdString());


//    //12343
//    //11319
//    //10807
//    //10551
//    //10039
//    //10039
//    //10551
//    //10551
//    //11063
//    //11575
//    //11831
//    changeEndianness(12343);
//    changeEndianness(11319);
//    changeEndianness(10807);
//    changeEndianness(10551);
//    changeEndianness(10039);
//    changeEndianness(10551);
//    changeEndianness(11063);
//    changeEndianness(11575);
//    changeEndianness(11831);


//}


void oneImageAtATime(tgfsTabbedWindow &w)
{
    flirImgWindow *fW = new flirImgWindow();
=======
#include "tgfsmainwindow.h"
#include <QThread>


void oneImageAtATime(tgfsTabbedWindow &w)
{
    flirbabaWindow *fW = new flirbabaWindow();
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
    segmentorWindow *sW = new segmentorWindow();

    w.addView(fW, QString("Flirbaba Viewer"));
    w.addView(sW, QString("Segmentor"));


    w.loadDefaultImage();
    w.connectEverythingForOneImage();
}

<<<<<<< HEAD
//void multipleImagesAtATime(tgfsTabbedWindow &w)
//{
//    w.load5Images();
//    w.connectEverythingFor5Images();
//}
=======
void multipleImagesAtATime(tgfsTabbedWindow &w)
{
    w.load5Images();
    w.connectEverythingFor5Images();
}
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create tabbed views
    tgfsTabbedWindow w;
    w.setupMenubars();
    oneImageAtATime(w);
    //multipleImagesAtATime(w);

    w.showMaximized();

<<<<<<< HEAD
    //testOnlyFlirImg();

=======
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
    return a.exec();
}
