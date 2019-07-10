#ifndef PROGRESSBARFRAME_H
#define PROGRESSBARFRAME_H

#include <QSlider>
#include <QStatusBar>
#include <QPushButton>
#include <QGraphicsPixmapItem>
#include <QProgressBar>
#include <QGridLayout>

#include "opencv_headers.h"
#include "segmentor.h"
//#include "graphicsscene.h"
using namespace cv;

class progressBarFrame : public QFrame
{
    Q_OBJECT
public:
    explicit progressBarFrame(QWidget *parent = 0);
public slots:
    //void updateProcessedImage(cv::Mat img);
signals:
    void signalProgress(int);
private slots:

private:
    QProgressBar *pBar;
};


#endif // PROGRESSBARFRAME_H
