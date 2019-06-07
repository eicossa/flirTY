#include "segmentorframe.h"
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>

segmentorFrame::segmentorFrame(const QString &name,
                               const cv::Mat img,
                               QWidget *parent)
       : QFrame(parent)
{
    qDebug() << "segmentorFrame : segmentorFrame() -- Constructor begun ";
    setFrameStyle(Sunken | StyledPanel);
    setLineWidth(5);

    setupImgDisplayer(img);
    this->frameName = new QString(name);
    QGridLayout *topLayout = new QGridLayout;
    topLayout->addWidget(segmentorImgDisplayer);

    setLayout(topLayout);
    qDebug() << "segmentorFrame : segmentorFrame() -- Constructor done ";
}

//A Mapping of Type to Numbers in OpenCV

//          C1	C2	C3	C4
//CV_8U	    0	8	16	24
//CV_8S	    1	9	17	25
//CV_16U	2	10	18	26
//CV_16S	3	11	19	27
//CV_32S	4	12	20	28
//CV_32F	5	13	21	29
//CV_64F	6	14	22	30

void segmentorFrame::convertMatTypeToQImageType(cv::Mat img, QImage &qimg)
{
    if(img.type() == 16){
        //qDebug()<<"Built "<<frameName<<" frame for type CV_8UC3";
        qimg = QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    } else if(img.type() == 0){
        //qDebug()<<"In here";
        qDebug() << "segmentorFrame -- convertMatTypeToQImageType() -- Built "
                 << frameName << " frame for type CV_8UC1";
        qimg = QImage(img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);
    } else if(img.type() == 5){
        //qDebug()<<"Built "<<frameName<<" frame for type CV_32FC1";
        qimg = QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB32);
    } else if(img.type() == 4){
        //qDebug()<<"Built "<<frameName<<" frame for type CV_32SC1";
        qimg = QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB32);
    }
    else{
        //qDebug()<<"Weirdo format : "<<img.type();
    }
}

void segmentorFrame::setupImgDisplayer(cv::Mat img)
{
    origScene = new QGraphicsScene(this);
    origScene->addItem(&originalPixmap);
    this->convertMatTypeToQImageType(img, originalImage);
    originalPixmap.setPixmap(QPixmap::fromImage(originalImage.rgbSwapped()));

    segmentorImgDisplayer = new segmentorImgDisplayerGraphicsView(this);
    segmentorImgDisplayer->setRenderHint(QPainter::Antialiasing, false);
    segmentorImgDisplayer->setDragMode(QGraphicsView::RubberBandDrag);
    segmentorImgDisplayer->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    segmentorImgDisplayer->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    segmentorImgDisplayer->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    segmentorImgDisplayer->setMouseTracking(true);
    segmentorImgDisplayer->setScene(origScene);
}

void segmentorFrame::setupSlots()
{
    connect(processButton, SIGNAL(clicked(bool)), this, SLOT(updateProcessedImage()));
}

QGraphicsView *segmentorFrame::getImgDisplayer() const
{
    return static_cast<QGraphicsView *>(segmentorImgDisplayer);
}

void segmentorFrame::updateProcessedImage(cv::Mat processedMat)
{
    qDebug() << "segmentorFrame : updateProcessedImage() -- Updating the processed image";
    QGraphicsScene *processedScene;
    processedScene = new QGraphicsScene(this);
    processedScene->addItem(&processedPixmap);
    this->convertMatTypeToQImageType(processedMat, processedImage);
    processedPixmap.setPixmap(QPixmap::fromImage(processedImage.rgbSwapped()));
    //qDebug()<<"Updating image of type : "<<processedMat.type();
    this->getImgDisplayer()->setScene(processedScene);
    qDebug() << "segmentorFrame : updateProcessedImage() -- Done with updating the processed image";
}
