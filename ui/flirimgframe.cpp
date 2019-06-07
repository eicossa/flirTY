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
        double zoomFactor = qPow(1.0015, angleDeltaY);
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

flirImgFrame::flirImgFrame(const QString &name, flirImg *fimg, Segmentor *segmentor, QWidget *parent)
    : QFrame(parent)
{
    qDebug() << "flirImgFrame : flirImgFrame() constructor begun";
    setFrameStyle(Sunken | StyledPanel);

    isLeafDetectionMode = false;

    this->fimg      = fimg;
    this->segmentor = segmentor;
    this->imgWidth  = fimg->getRAWThermalImageWidth();
    this->imgHeight = fimg->getRAWThermalImageHeight();

    setupImgDisplayer();

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    iconSize = new QSize(size, size);

    setupRotateStuff();
    setupZoomStuff();

    setupViewingModes();
    setupProcessingModes();

    label = new QLabel(name);
    layoutEverything();

    connectEverything();
    setupMatrix();

    qDebug() << "flirImgFrame : flirImgFrame() constructor done";
}


void flirImgFrame::layoutEverything()
{

    setupTopButtonsLayout();

    resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);

    statusBar = new QStatusBar;
    statusBar->setStatusTip(tr("This is the status bar"));

    lcdDisplay1Label = new QLabel();
    lcdDisplay1 = new QLCDNumber();
    lcdDisplay1->setDigitCount(6);
    lcdDisplay1->setMinimumWidth(300);

    lcdDisplay2Label = new QLabel();
    lcdDisplay2 = new QLCDNumber();
    lcdDisplay2->setDigitCount(6);
    lcdDisplay2->setMinimumWidth(300);


    QVBoxLayout *tempDisplaysLayout = new QVBoxLayout;
    tempDisplaysLayout->addWidget(lcdDisplay1Label);
    tempDisplaysLayout->addWidget(lcdDisplay1);

    tempDisplaysLayout->addWidget(lcdDisplay2Label);
    tempDisplaysLayout->addWidget(lcdDisplay2);





    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(topButtonsLayout, 0, 0);
    topLayout->addWidget(flirImgDisplayer, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addLayout(tempDisplaysLayout, 1, 2);
    topLayout->addLayout(rotateSliderLayout, 2, 0);
    topLayout->addWidget(resetButton, 2, 1);
    topLayout->addWidget(statusBar, 3, 0);

    setLayout(topLayout);
}

void flirImgFrame::restoreOriginalImage()
{
    baseScene = new QGraphicsScene(this);
    baseScene->addItem(&basePixmap);
    this->baseMat = imread(fimg->getDisplayRAWsImgpath(), cv::IMREAD_COLOR);
    baseImage = QImage(baseMat.data,
                           baseMat.cols, baseMat.rows,
                           baseMat.step,
                           QImage::Format_RGB888);
    basePixmap.setPixmap(QPixmap::fromImage(baseImage.rgbSwapped()));

    flirImgDisplayer->setScene(baseScene);
}

void flirImgFrame::updateBaseMat()
{
    // this->baseMat = imread(fimg->getThermalImgPath(), cv::IMREAD_COLOR);
    if(rawImageViewButton->isChecked()){
        qDebug() << " updateBaseMat() : baseMat is now thermal waala";
        this->baseMat = imread(fimg->getDisplayRAWsImgpath(), cv::IMREAD_COLOR);
    } else if(origImageViewButton->isChecked()){
        qDebug() << " updateBaseMat() : baseMat is now orig waala";
        this->baseMat = imread(fimg->getImgPath(), cv::IMREAD_COLOR);
        cv::Mat resizedMat;
        //cv::resize(baseMat, resizedMat, (fimg->getThermalImgMat()).size(), 0, 0, INTER_LINEAR );
    } else if(repalettedImageViewButton->isChecked()){
        qDebug() << " updateBaseMat() : baseMat is now repaletted waala";
        this->baseMat = imread(fimg->getRepalettedImgPath(), cv::IMREAD_COLOR);
    }
}


void flirImgFrame::createBaseMat()
{
    this->baseMat = imread(fimg->getDisplayRAWsImgpath(), cv::IMREAD_COLOR);
}

void flirImgFrame::createBasePixmap()
{
    baseImage = QImage(baseMat.data,
                       baseMat.cols, baseMat.rows,
                       baseMat.step,
                       QImage::Format_RGB888);
    basePixmap.setPixmap(QPixmap::fromImage(baseImage.rgbSwapped()));
}


void flirImgFrame::createBaseScene()
{
    baseScene = new QGraphicsScene(this);
    baseScene->addItem(&basePixmap);
}

void flirImgFrame::displayUpdatedBaseScene()
{
    updateBaseMat();
    createBasePixmap();
    createBaseScene();
    flirImgDisplayer->setScene(baseScene);
}

void flirImgFrame::displayBaseScene()
{
    createBaseMat();
    createBasePixmap();
    createBaseScene();
    flirImgDisplayer->setScene(baseScene);
}

void flirImgFrame::setupImgDisplayer()
{
    flirImgDisplayer = new flirImgDisplayerGraphicsView(this);
    flirImgDisplayer->setRenderHint(QPainter::Antialiasing, false);
    flirImgDisplayer->setDragMode(QGraphicsView::RubberBandDrag);
    flirImgDisplayer->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    flirImgDisplayer->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    flirImgDisplayer->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    flirImgDisplayer->setMouseTracking(true); //to be able to retrieve image pixel coords

    displayBaseScene();
}

void flirImgFrame::createOverlayMat()
{
    overlay_alpha = (double)((double)transparencySlider->value())/(10.0);
    overlayMatOutput = baseMat.clone();

    cv::addWeighted(baseMat,    1.0 - overlay_alpha,
                    overlayMat, overlay_alpha,
                    0.0,
                    overlayMatOutput);
}

void flirImgFrame::createOverlayPixmap()
{
    overlayImage = QImage(overlayMatOutput.data,
                          overlayMatOutput.cols, overlayMatOutput.rows,
                          overlayMatOutput.step,
                          QImage::Format_RGB888);
    overlayPixmap.setPixmap(QPixmap::fromImage(overlayImage.rgbSwapped()));
}

void flirImgFrame::createOverlayScene()
{
    overlayScene = new QGraphicsScene(this);
    overlayScene->addItem(&overlayPixmap);
}


void flirImgFrame::displayTransparentBlobOverlayScene()
{
    createOverlayMat();
    createOverlayPixmap();
    createOverlayScene();

    flirImgDisplayer->setScene(overlayScene);
}


void flirImgFrame::updateWithOverlay(cv::Mat overlay)
{
    overlayMat   = overlay;
    displayTransparentBlobOverlayScene();
}


void flirImgFrame::setupViewingModes()
{
    rawImageViewButton = new QToolButton;
    rawImageViewButton->setText(tr("Raw"));
    rawImageViewButton->setCheckable(true);
    rawImageViewButton->setChecked(true);

    origImageViewButton = new QToolButton;
    origImageViewButton->setText(tr("Original"));
    origImageViewButton->setCheckable(true);
    origImageViewButton->setChecked(false);

    repalettedImageViewButton = new QToolButton;
    repalettedImageViewButton->setText(tr("Repaletted"));
    repalettedImageViewButton->setCheckable(true);
    repalettedImageViewButton->setChecked(false);

    QButtonGroup *pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(rawImageViewButton);
    pointerModeGroup->addButton(origImageViewButton);
    pointerModeGroup->addButton(repalettedImageViewButton);
}

void flirImgFrame::setupProcessingModes()
{
    leafDetectButton = new QToolButton;
    leafDetectButton->setText(tr("LeafDetect"));
    leafDetectButton->setCheckable(true);
    leafDetectButton->setChecked(false);

    // rectSelectButton = new QToolButton;
    // rectSelectButton->setText(tr("RectSelect"));
    // rectSelectButton->setCheckable(true);
    // rectSelectButton->setChecked(false);


    pointSelectButton = new QToolButton;
    pointSelectButton->setText(tr("PointSelect"));
    pointSelectButton->setCheckable(true);
    pointSelectButton->setChecked(true);

    // printButton = new QToolButton;
    // printButton->setIcon(QIcon(QPixmap(":/chips-images/fileprint.png")));

    QButtonGroup *processingModeGroup = new QButtonGroup(this);
    processingModeGroup->setExclusive(true);
    processingModeGroup->addButton(leafDetectButton);
    //processingModeGroup->addButton(rectSelectButton);
    processingModeGroup->addButton(pointSelectButton);
}

void flirImgFrame::connectEverything()
{
    connect(resetButton,        SIGNAL(clicked()),         this, SLOT(resetView()));
    connect(zoomSlider,         SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(transparencySlider, SIGNAL(valueChanged(int)), this, SLOT(displayTransparentBlobOverlayScene()));
    connect(rotateSlider,       SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));

    connect(rawImageViewButton,  SIGNAL(toggled(bool)),    this, SLOT(toggleImageDisplayMode()));
    connect(origImageViewButton, SIGNAL(toggled(bool)),    this, SLOT(toggleImageDisplayMode()));
    connect(repalettedImageViewButton, SIGNAL(toggled(bool)), this, SLOT(toggleImageDisplayMode()));

    connect(leafDetectButton,   SIGNAL(toggled(bool)),     this, SLOT(toggleLeafButtonStatus()));
    connect(pointSelectButton,  SIGNAL(toggled(bool)),     this, SLOT(togglePointButtonStatus()));

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
            this,                                          SLOT(receiveStatusBarInfo(QPoint)));
}

void flirImgFrame::setupZoomStuff()
{
    zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/chips-images/zoomin.png"));
    zoomInIcon->setIconSize(*iconSize);

    zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/chips-images/zoomout.png"));
    zoomOutIcon->setIconSize(*iconSize);

    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    transparencySlider = new QSlider;
    transparencySlider->setMinimum(0);
    transparencySlider->setMaximum(10);
    transparencySlider->setValue(5);
    transparencySlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(transparencySlider);
    zoomSliderLayout->addWidget(zoomOutIcon);
}

void flirImgFrame::setupRotateStuff()
{
    rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/chips-images/rotateleft.png"));
    rotateLeftIcon->setIconSize(*iconSize);

    rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/chips-images/rotateright.png"));
    rotateRightIcon->setIconSize(*iconSize);

    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);
}

void flirImgFrame::setupTopButtonsLayout()
{
    // Label layout
    topButtonsLayout = new QHBoxLayout;
    label2 = new QLabel(tr("Image Display Mode"));

    // labelLayout->addWidget(label);
    // labelLayout->addStretch();
    topButtonsLayout->addWidget(label2);
    topButtonsLayout->addWidget(rawImageViewButton);
    topButtonsLayout->addWidget(origImageViewButton);
    topButtonsLayout->addWidget(repalettedImageViewButton);
    topButtonsLayout->addStretch();
    topButtonsLayout->addWidget(pointSelectButton);
    topButtonsLayout->addWidget(leafDetectButton);
    // topButtonsLayout->addWidget(rectSelectButton);
    // topButtonsLayout->addWidget(printButton);
}

QGraphicsView *flirImgFrame::view() const
{
    return static_cast<QGraphicsView *>(flirImgDisplayer);
}

void flirImgFrame::resetView()
{
    zoomSlider->setValue(250);
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

void flirImgFrame::toggleImageDisplayMode()
{
//    flirImgDisplayer->setDragMode(rawImageViewButton->isChecked()
//                              ? QGraphicsView::RubberBandDrag
//                              : QGraphicsView::ScrollHandDrag);
//    flirImgDisplayer->setInteractive(rawImageViewButton->isChecked());

    if(rawImageViewButton->isChecked() && isLeafDetectionMode){
        qDebug() << " flirImgFrame : toggleImageDisplayMode() : display rawImg with leafDetection overlay -- raw view";
        updateBaseMat();
        displayTransparentBlobOverlayScene();
    } else if(rawImageViewButton->isChecked() && !isLeafDetectionMode){
        qDebug() << " flirImgFrame : toggleImageDisplayMode() : display rawImg without leafDetection -- raw view";
        updateBaseMat();
        displayUpdatedBaseScene();
    } else if(origImageViewButton->isChecked() && isLeafDetectionMode){
        qDebug() << " flirImgFrame : toggleImageDisplayMode() : display origImg with leafDetection overlay -- orig view";
        updateBaseMat();
        displayTransparentBlobOverlayScene();
    } else if(origImageViewButton->isChecked() && !isLeafDetectionMode){
        qDebug() << " flirImgFrame : toggleImageDisplayMode() : display origImg without leafDetection -- raw view";
        updateBaseMat();
        displayUpdatedBaseScene();
    } else if(repalettedImageViewButton->isChecked() && isLeafDetectionMode){
        qDebug() << " flirImgFrame : toggleImageDisplayMode() : display repalettedImg with leafDetection overlay -- repaletted view";
        updateBaseMat();
        displayTransparentBlobOverlayScene();
    } else if(repalettedImageViewButton->isChecked() && isLeafDetectionMode){
        qDebug() << " flirImgFrame : toggleImageDisplayMode() without leafDetection -- raw view";
        updateBaseMat();
        displayUpdatedBaseScene();
    }
}

void flirImgFrame::toggleLeafButtonStatus()
{
    //flirImgDisplayer->setRenderHint(QPainter::Antialiasing, leafDetectButton->isChecked());
    qDebug() << " flirImgFrame::toggleLeafButtonStatus() - Leaf Detect Button Clicked ";
    if(leafDetectButton->isChecked()){
        isLeafDetectionMode = true;
        emit leafButtonStatus(true);
        qDebug() << " flirImgFrame::toggleLeafButtonStatus() - emitted leafButtonStatus(true) signal";
    }
    else{
        isLeafDetectionMode = false;
        emit leafButtonStatus(false);
        qDebug() << " flirImgFrame::toggleLeafButtonStatus() - emitted leafButtonStatus(false) signal";
    }
}

void flirImgFrame::zoomIn(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void flirImgFrame::zoomOut(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
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

void flirImgFrame::receiveStatusBarInfo(QPoint imgPos)
{
    qDebug() << QString("flirImgFrame::receiveStatusBarInfo(%1, %2) - %3x%4").arg(imgPos.x()).arg(imgPos.y()).arg(imgWidth).arg(imgHeight);
    int colNum = imgPos.x();
    int rowNum = imgPos.y();


    if(colNum < 0 || rowNum < 0 || colNum > imgWidth || rowNum > imgHeight){
        statusBar->showMessage("Not over image");
        lcdDisplay1->display(-1);
        lcdDisplay2->display(-1);
        lcdDisplay1Label->setText("--");
        lcdDisplay2Label->setText("--");
    }
    else{
      if(isLeafDetectionMode){
          statusBar->showMessage(segmentor->getStatusString(imgPos));
          //tempDisplay->setFont();
          lcdDisplay1Label->setText("Blob Area");
          lcdDisplay1->display(segmentor->getIsPointKaBlobArea(imgPos));
          lcdDisplay2Label->setText("Blob Avg Temp");
          lcdDisplay2->display(segmentor->getIsPointKeBlobKaAvgTemp(imgPos));
      }
      else{
        // qDebug() << " flirImgFrame::receiveStatusBarInfo -- Trying to show "
        //          << fimg->getStatusString(posX, posY);
        statusBar->showMessage(fimg->getStatusString(rowNum, colNum));
        lcdDisplay1Label->setText("RAW Sensor Value");
        lcdDisplay1->display(fimg->getOrderedRAWval(rowNum, colNum));
        lcdDisplay2Label->setText("Temperature");
        lcdDisplay2->display(fimg->getPixelTemperature(rowNum, colNum));

      }
    }
}

void flirImgFrame::rotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
}

void flirImgFrame::rotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);
}

void flirImgFrame::updateStatusBar()
{
    //qDebug()<<"Status bar shall be updated";
}

void flirImgFrame::setFlirImg(flirImg *fimg)
{
    this->fimg = fimg;
}

void flirImgFrame::setImg(QImage *img)
{
    this->img = img;
}
