#ifndef SEGMENTORFRAME_H
#define SEGMENTORFRAME_H

#include <QObject>
#include <QFrame>
#include <QGraphicsView>

class segmentorFrame;

class segmentorImgDisplayerGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    segmentorImgDisplayerGraphicsView(segmentorFrame *v) : QGraphicsView(), sframe(v) { }
signals:
    void statusBarSignal(QPointF imgPos);
protected:
    //void wheelEvent(QWheelEvent *) override;
    //void mouseMoveEvent(QMouseEvent *) override;
    //void mousePressEvent(QMouseEvent *) override;
    //void mouseReleaseEvent(QMouseEvent *) override;

private:
    segmentorFrame *sframe;

};


#include <QSlider>
#include <QStatusBar>
#include <QPushButton>
#include <QGraphicsPixmapItem>

#include <opencv2/opencv.hpp>
#include "segmentor.h"
//#include "graphicsscene.h"
using namespace cv;

class segmentorFrame : public QFrame
{
    Q_OBJECT
public:
    explicit segmentorFrame(const QString &name, const cv::Mat img, QWidget *parent = 0);
    QGraphicsView *getImgDisplayer() const;
public slots:
    void updateProcessedImage(cv::Mat img);

private slots:

    //void updateStatusBar();

private:
    void setupImgDisplayer(cv::Mat img);
    void connectEverything();
    void readImage(std::string fileName);
    void convertMatTypeToQImageType(cv::Mat, QImage&);
    QString *frameName;
    segmentorImgDisplayerGraphicsView *segmentorImgDisplayer;
    QPushButton *processButton;

    QSlider *leafParam1Slider;
    QSlider *leafParam2Slider;
    QSlider *leafParam3Slider;

    QGraphicsPixmapItem originalPixmap, processedPixmap;
    cv::Mat originalMat, processedMat;
    QImage originalImage, processedImage;

    QGraphicsScene *origScene;
    QStatusBar *statusBar;
};

#endif // SEGMENTORFRAME_H
