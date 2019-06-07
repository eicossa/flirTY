#ifndef FLIRIMGFRAME_H
#define FLIRIMGFRAME_H

#include <QFrame>
#include <QGraphicsView>
#include <QLCDNumber>
#include <QGraphicsPixmapItem>
#include "flirimg.h"
#include "segmentor.h"

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
    void leafButtonStatus(bool);
//    void blobAvgTempBataaoIsPointKa(QPoint imgPos);
    void blobIDBataaoIsPointKa(QPoint imgPos);
//    void blobAreaBataaoIsPointKa(QPoint imgPos);
public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);
    void receiveStatusBarInfo(QPoint imgPos);
    void updateBlobAvgTemp(double d);
    void updateBlobArea(int a);
private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void toggleImageDisplayMode();

    void toggleLeafButtonStatus();
    void displayTransparentBlobOverlayScene();

    void rotateLeft();
    void rotateRight();
    void updateStatusBar();
public:
    explicit flirImgFrame(const QString &name,
                          flirImg *fimg, Segmentor *segmentor,
                          QWidget *parent = 0);
    void     setFlirImg(flirImg *fimg);
    void     setImg(QImage *img);
    QGraphicsView *view() const;
    QToolButton*  getLeafDetectButton() { return leafDetectButton; }
    void          updateWithOverlay(cv::Mat);
    void          restoreOriginalImage();

private:
    void setupImgDisplayer();

    void displayBaseScene();
    void displayUpdatedBaseScene();

    void createBaseMat();
    void createBaseScene();
    void createBasePixmap();

    void layoutEverything();

    void createOverlayMat();
    void createOverlayScene();
    void createOverlayPixmap();

    void updateBaseMat();

    flirImgDisplayerGraphicsView *flirImgDisplayer;
    flirImg                      *fimg;
    Segmentor                    *segmentor;
    QImage                       *img;
    int                           imgWidth, imgHeight;
    double                        mousedOverBlobAvgTemp;
    int                           mousedOverBlobArea;
    double                        overlay_alpha;

    bool                          isLeafDetectionMode;

    QLabel                       *label;
    QLabel                       *label2;
    QLabel *lcdDisplay1Label;
    QLabel *lcdDisplay2Label;

    void                          setupViewingModes();
    QToolButton                  *rawImageViewButton;
    QToolButton                  *origImageViewButton;
    QToolButton                  *repalettedImageViewButton;

    void                          setupProcessingModes();
    QToolButton *leafDetectButton;
    QToolButton *rectSelectButton;
    QToolButton *pointSelectButton;

    QToolButton *resetButton;

    void        connectEverything();

    QSize       *iconSize;

    void         setupZoomStuff();
    QToolButton *zoomInIcon;
    QToolButton *zoomOutIcon;
    QSlider     *zoomSlider;
    QSlider     *transparencySlider;
    QVBoxLayout *zoomSliderLayout;

    void         setupRotateStuff();
    QToolButton *rotateLeftIcon;
    QToolButton *rotateRightIcon;
    QSlider     *rotateSlider;
    QHBoxLayout *rotateSliderLayout;

    void         setupTopButtonsLayout();
    QHBoxLayout *topButtonsLayout;


    QSlider *leafParam1Slider;
    QSlider *leafParam2Slider;
    QSlider *leafParam3Slider;


    QStatusBar *statusBar;
    QLCDNumber          *lcdDisplay1;
    QLCDNumber          *lcdDisplay2;


    QGraphicsPixmapItem basePixmap, overlayPixmap;
    cv::Mat             baseMat, overlayMat, overlayMatOutput;
    cv::Mat             rawMat, rgbMat, repalettedMat, paletteMat;
    QImage              baseImage, overlayImage;

    QGraphicsScene *baseScene;
    QGraphicsScene *overlayScene;

};


#endif // FLIRIMGFRAME_H
