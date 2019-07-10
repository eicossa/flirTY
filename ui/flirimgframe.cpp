//#ifndef QT_NO_OPENGL
//#include <QtOpenGL>
//#else
#include <QtWidgets>
#include "flirimgframe.h"
//#endif
#include <qmath.h>

void flirImgDisplayerGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        double angleDeltaY = event->angleDelta().y();
        double zoomFactor  = qPow(1.0015, angleDeltaY);
        scale(zoomFactor, zoomFactor);
        if(angleDeltaY > 0)
        {
            this->centerOn(sceneMousePos);
            sceneMousePos = this->mapToScene(event->pos());
        }
        else{
            this->centerOn(sceneMousePos);
            sceneMousePos = this->mapToScene(event->pos());
        }
        this->viewport()->update();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
//#endif

void flirImgDisplayerGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QString text;
    text = QString("%1 x %2").arg(event->pos().x()).arg(event->pos().y());
    sceneMousePos = this->mapToScene(event->pos());
    //    qDebug() << "flirImgDisplayerGraphicsView::mouseMoveEvent "
    //             << text
    //             << " : "
    //             << sceneMousePos;

    // QPoint origin;
    // origin = event->pos();
    // if(rubberBand)
    //   rubberBand->setGeometry(QRect(origin, event->pos()).normalized());

    emit mouseMovedPositionSignal(sceneMousePos.toPoint());
}

void flirImgDisplayerGraphicsView::mousePressEvent(QMouseEvent *event)
{
//    QPoint origin = event->pos();
//    if (!rubberBand)
//        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
//    rubberBand->setGeometry(QRect(origin, QSize()));
//    rubberBand->show();
}

void flirImgDisplayerGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    //rubberBand->hide();
    // determine selection, for example using QRect::intersects()
    // and QRect::contains().
}

flirImgFrame::flirImgFrame(const QString &name, flirImg *fimg, QWidget *parent)
    : QFrame(parent)
{
    qDebug() << "flirImgFrame : flirImgFrame() constructor begun";
    setFrameStyle(Sunken | StyledPanel);

    isLeafDetectionMode = false;

    this->fimg      = fimg;
    this->segmentor = fimg->getSegmentorObject();
    this->imgWidth  = fimg->getRAWThermalImageWidth();
    this->imgHeight = fimg->getRAWThermalImageHeight();

    UiSetupImgDisplayer();

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    iconSize = new QSize(size, size);

    UiSetupRotateStuff();
    UiSetupZoomStuff();

    UiSetupViewingModes();
    UiSetupProcessingModes();

    label = new QLabel(name);
    UiLayoutEverything();

    connectEverything();
    setupMatrix();

    displayOriginalImage();
    //qDebug() << "flirImgFrame : flirImgFrame() constructor done";
}

void flirImgFrame::changeUpperRange(int upperCutoff)
{
    qDebug() << "flirImgFrame::changeUpperRange - Upper value changed to " << upperCutoff;
    fimg->getRangedMat(tempRangeSlider->GetLowerValue(), upperCutoff, isRepalettedImageViewMode);
    displayRangedMat();
}

void flirImgFrame::changeLowerRange(int lowerCutoff)
{
    qDebug() << "flirImgFrame::changeLowerRange - Lower value changed to " << lowerCutoff;
    fimg->getRangedMat(lowerCutoff, tempRangeSlider->GetUpperValue(), isRepalettedImageViewMode);
    displayRangedMat();
}


void flirImgFrame::connectEverything()
{
    connect(tempRangeSlider,    SIGNAL(lowerValueChanged(int)),
            this,               SLOT(changeLowerRange(int)));
    connect(tempRangeSlider,    SIGNAL(upperValueChanged(int)),
            this,               SLOT(changeUpperRange(int)));


    connect(resetButton,        SIGNAL(clicked()),         this, SLOT(resetView()));
    connect(rotateSlider,       SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));

    connect(rawImageViewButton,  SIGNAL(toggled(bool)),    this, SLOT(displayImage()));
    connect(origImageViewButton, SIGNAL(toggled(bool)),    this, SLOT(displayImage()));
    connect(repalettedImageViewButton, SIGNAL(toggled(bool)), this, SLOT(displayImage()));

    connect(leafDetectButton,   SIGNAL(toggled(bool)),     this, SLOT(displayImage()));
    connect(pointSelectButton,  SIGNAL(toggled(bool)),     this, SLOT(displayImage()));

    // connect(rectSelectButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
    connect(rotateLeftIcon,     SIGNAL(clicked()),         this, SLOT(rotateLeft()));
    connect(rotateRightIcon,    SIGNAL(clicked()),         this, SLOT(rotateRight()));
    connect(zoomInIcon,         SIGNAL(clicked()),         this, SLOT(zoomIn()));
    connect(zoomOutIcon,        SIGNAL(clicked()),         this, SLOT(zoomOut()));

    connect(flirImgDisplayer->verticalScrollBar(),         SIGNAL(valueChanged(int)),
            this,                                          SLOT(setResetButtonEnabled()));
    connect(flirImgDisplayer->horizontalScrollBar(),       SIGNAL(valueChanged(int)),
            this,                                          SLOT(setResetButtonEnabled()));
    connect(flirImgDisplayer,                              SIGNAL(mouseMovedPositionSignal(QPoint)),
            this,                                          SLOT(displayPointInfoOnStatusBar(QPoint)));


}

QGraphicsView *flirImgFrame::view() const
{
    return static_cast<QGraphicsView *>(flirImgDisplayer);
}

void flirImgFrame::updateOverlayTransparency(int alpha)
{
    if(isLeafDetectionMode) displayOverlaidImage();
}

void flirImgFrame::resetView()
{
    //rangeSlider->setValue(250);
    rotateSlider->setValue(0);
    setupMatrix();
    flirImgDisplayer->ensureVisible(QRectF(0, 0, 0, 0));
    resetButton->setEnabled(false);
}

void flirImgFrame::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void flirImgFrame::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));
    // qDebug()<<"Scale : "<<scale<<" | Zoom value : "<<(zoomSlider->value());

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(rotateSlider->value());

    flirImgDisplayer->setMatrix(matrix);
    setResetButtonEnabled();
}

void flirImgFrame::createSceneFromMat(Mat imgMat)
{
    imgScene = new QGraphicsScene(this);
    QImage::Format imgType;
    if(imgMat.type() == 16)
        imgType = QImage::Format_RGB888;
    if(imgMat.type() == 0)
        imgType = QImage::Format_Grayscale8;

    imgImage = QImage(imgMat.data,
                      imgMat.cols, imgMat.rows,
                      imgMat.step,
                      imgType);
    imgPixmap.setPixmap(QPixmap::fromImage(imgImage.rgbSwapped()));
    imgScene->addItem(&imgPixmap);
}

void flirImgFrame::displayOverlaidImage()
{
    overlay_alpha    = (double)((double)transparencySlider->value())/(10.0);

    overlaidMat = fimg->getTransparencyOverlaidMat(overlay_alpha);
    createSceneFromMat(overlaidMat);
    flirImgDisplayer->setScene(imgScene);
}

void flirImgFrame::displayOriginalImage()
{
    baseMat = fimg->getGrayscaleThermalMat().clone();
    createSceneFromMat(baseMat);
    flirImgDisplayer->setScene(imgScene);
}

void flirImgFrame::displayRangedMat()
{
    int l, u;
    l = tempRangeSlider->GetLowerValue();
    u = tempRangeSlider->GetUpperValue();
    baseMat = fimg->getRangedMat(l, u, isRepalettedImageViewMode).clone();
    //baseMat = fimg->getRangedThermalMat().clone();
    createSceneFromMat(baseMat);
    flirImgDisplayer->setScene(imgScene);
}

void flirImgFrame::displayImage()
{
    //    flirImgDisplayer->setDragMode(rawImageViewButton->isChecked()
    //                              ? QGraphicsView::RubberBandDrag
    //                              : QGraphicsView::ScrollHandDrag);
    //    flirImgDisplayer->setInteractive(rawImageViewButton->isChecked());
    isLeafDetectionMode       = leafDetectButton->isChecked()          ? true : false;
    isOrigImageViewMode       = origImageViewButton->isChecked()       ? true : false;
    isRawImageViewMode        = rawImageViewButton->isChecked()        ? true : false;
    isRepalettedImageViewMode = repalettedImageViewButton->isChecked() ? true : false;

    if(!isLeafDetectionMode){
        if(isRawImageViewMode)        baseMat = fimg->getGrayscaleThermalMat().clone();
        if(isOrigImageViewMode)       baseMat = fimg->getOrigImgMat().clone();
        if(isRepalettedImageViewMode) baseMat = fimg->getRepalettedImgMat().clone();
        createSceneFromMat(baseMat);
        flirImgDisplayer->setScene(imgScene);
    }
    if(isLeafDetectionMode){
        qDebug() << "flirImgFrame::displayImage -- About to update overlay";
        displayOverlaidImage();
        qDebug() << "flirImgFrame::displayImage -- Done with overlay update";
    }
}

void flirImgFrame::changePalette(int c)
{
    cv::ColormapTypes d = (cv::ColormapTypes)c;
    fimg->changePalette(d);
    displayImage();
}

void flirImgFrame::updateBlobAvgTemp(double d)
{
    qDebug() << "flirImgFrame::updateBlobAvgTemp(double) : "
             << "blobAvgTemp has been updated ... ";
    mousedOverBlobAvgTemp = d;
}

void flirImgFrame::updateBlobArea(int area)
{
    qDebug() << "flirImgFrame::updateBlobAvgTemp(double) : "
             << "updateBlobArea has been updated ... ";
    mousedOverBlobArea = area;
}

void flirImgFrame::updateLCDDisplays()
{
    if(!isMouseOverImage){
        lcdDisplay1->display(-1);
        lcdDisplay2->display(-1);
        lcdDisplay1Label->setText("--");
        lcdDisplay2Label->setText("--");
    }
    else{
        if(isLeafDetectionMode){
            lcdDisplay1Label->setText("Blob Area");
            lcdDisplay1->display(segmentor->getIsPointKaBlobArea(mousePos));
            lcdDisplay2Label->setText("Blob Avg Temp");
            lcdDisplay2->display(fimg->calcBlobAvgTemp(segmentor->getIsPointKeBlobKePoints(mousePos)));
        }
        else{
            lcdDisplay1Label->setText("RAW Sensor Value");
            lcdDisplay1->display(fimg->getOrderedRAWval(mousePosRowNum, mousePosColNum));
            lcdDisplay2Label->setText("Temperature");
            lcdDisplay2->display(fimg->getPixelTemperature(mousePosRowNum, mousePosColNum));
        }
    }
}

void flirImgFrame::displayPointInfoOnStatusBar(QPoint imgPos)
{
    // qDebug() << QString("flirImgFrame::receiveStatusBarInfo(%1, %2) - %3x%4")
    //             .arg(imgPos.x()).arg(imgPos.y())
    //             .arg(imgWidth).arg(imgHeight);
    mousePos       = imgPos;
    mousePosColNum = mousePos.x();
    mousePosRowNum = mousePos.y();

    if(mousePosColNum < 0 || mousePosRowNum < 0 || mousePosColNum > imgWidth || mousePosRowNum > imgHeight){
      statusBar->showMessage("Not over image");
      isMouseOverImage = false;
      updateLCDDisplays();
    }
    else{
      isMouseOverImage = true;
      if(isLeafDetectionMode){
        statusBar->showMessage(fimg->getSegmentorStatusString(imgPos));
        updateLCDDisplays();
      }
      else{
        statusBar->showMessage(fimg->getStatusString(mousePosRowNum, mousePosColNum));
        updateLCDDisplays();
      }
    }
}

void flirImgFrame::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 10);
}

void flirImgFrame::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 10);
}

void flirImgFrame::rotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
}

void flirImgFrame::rotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);
}

