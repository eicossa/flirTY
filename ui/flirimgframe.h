#ifndef FLIRIMGFRAME_H
#define FLIRIMGFRAME_H

#include <QFrame>
#include <QGraphicsView>
#include <QLCDNumber>
#include <QGraphicsPixmapItem>
#include "flirimg.h"
#include "segmentor.h"
#include "rangeslider.h"

class QLabel;
class QSlider;
class QToolButton;
class flirImgFrame;
class QHBoxLayout;
class QVBoxLayout;
class QStatusBar;
class flirImg;
class QRubberBand;

class flirImgDisplayerGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    flirImgDisplayerGraphicsView(flirImgFrame *v) : QGraphicsView(), fFrame(v) { }
signals:
    void mouseMovedPositionSignal(QPoint imgPos);
protected:
    void wheelEvent(QWheelEvent *)        override;
    void mouseMoveEvent(QMouseEvent *)    override;
    void mousePressEvent(QMouseEvent *)   override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    flirImgFrame *fFrame;
    QPointF      sceneMousePos;
    QRubberBand *rubberBand;
};


class flirImgFrame : public QFrame
{
    Q_OBJECT
signals:
    void                         leafButtonStatus(bool);
    void                         blobIDBataaoIsPointKa(QPoint imgPos);
    void                         changeOverlayAlpha(int);
public slots:
    void                         displayPointInfoOnStatusBar(QPoint imgPos);
    void                         updateBlobAvgTemp(double d);
    void                         updateBlobArea(int a);
    void                         changePalette(int);
private slots:
    void                         resetView();
    void                         setResetButtonEnabled();
    void                         setupMatrix();
    void                         displayImage();

    void                         updateOverlayTransparency(int);

    void                         rotateLeft();
    void                         rotateRight();
    void                         zoomIn();
    void                         zoomOut();
    void                         changeUpperRange(int);
    void                         changeLowerRange(int);
public:
    explicit flirImgFrame(const QString &name,
                          flirImg *fimg,
                          QWidget *parent = 0);
    QGraphicsView               *view() const;
    QToolButton*                 getLeafDetectButton() { return leafDetectButton; }
    void                         displayOverlaidImage();
    void                         displayOriginalImage();
    QSlider                       *transparencySlider; // i know this will come back to bite me in the ass
    RangeSlider                   *tempRangeSlider;
    QSlider                       *zoomSlider;

private:
    void                          UiLayoutEverything();
    void                          UiSetupImgDisplayer();
    void                          UiSetupViewingModes();
    void                          UiSetupProcessingModes();
    void                          UiSetupZoomStuff();
    void                          UiSetupRotateStuff();
    void                          UiSetupTopButtonsLayout();

    void                          updateLCDDisplays();

    void                          displayBaseScene();
    void                          createSceneFromMat(Mat);
    void                          createOverlayScene();
    void                          createOverlayPixmap();
    void                          displayRangedMat();

    flirImgDisplayerGraphicsView *flirImgDisplayer;
    flirImg                      *fimg;
    Segmentor                    *segmentor;
    QImage                       *img;
    int                           imgWidth, imgHeight;
    double                        mousedOverBlobAvgTemp;
    int                           mousedOverBlobArea;
    double                        overlay_alpha;

    bool                          isMouseOverImage;
    bool                          isLeafDetectionMode;
    bool                          isRepalettedImageViewMode;
    bool                          isOrigImageViewMode;
    bool                          isRawImageViewMode;

    QLabel                       *label;
    QLabel                       *imgDisplayModeLbl;
    QLabel                       *lcdDisplay1Label;
    QLabel                       *lcdDisplay2Label;


    QToolButton                  *rawImageViewButton;
    QToolButton                  *origImageViewButton;
    QToolButton                  *repalettedImageViewButton;

    QToolButton                   *leafDetectButton;
    QToolButton                   *rectSelectButton;
    QToolButton                   *pointSelectButton;
    QToolButton                   *resetButton;

    void                          connectEverything();

    QSize                         *iconSize;

    QToolButton                   *zoomInIcon, *zoomOutIcon;


    QVBoxLayout                   *zoomSliderLayout;

    QToolButton                   *rotateLeftIcon, *rotateRightIcon;
    QSlider                       *rotateSlider;
    QHBoxLayout                   *rotateSliderLayout;
    QHBoxLayout                   *topButtonsLayout;

    QSlider                       *leafParam1Slider;
    QSlider                       *leafParam2Slider;
    QSlider                       *leafParam3Slider;

    QStatusBar                    *statusBar;
    QLCDNumber                    *lcdDisplay1, *lcdDisplay2;


    //QGraphicsPixmapItem mainPixmap, overlayPixmap;
    QGraphicsPixmapItem           imgPixmap;
    cv::Mat                       baseMat, overlayMat, overlaidMat;
    cv::Mat                       rawMat, rgbMat, repalettedMat, paletteMat;
    //QImage              mainImage, overlayImage;

    QGraphicsScene                *imgScene;
    QImage                         imgImage;

    // to track the current mouse position over the image
    QPoint                         mousePos;
    int                            mousePosColNum, mousePosRowNum;

};


#endif // FLIRIMGFRAME_H
