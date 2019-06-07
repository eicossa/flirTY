#include "imgoperatorwindows.h"

imgOperatorWindows::imgOperatorWindows(QWidget *parent)
{
<<<<<<< HEAD
//    fW            = new flirImgWindow();
//    sW            = new segmentorWindow();
//    fimg          = new flirImg();
//    segmentor     = new Segmentor();
=======
    fW            = new flirbabaWindow();
    sW            = new segmentorWindow();
    fimg          = new flirImg();
    segmentor     = new Segmentor();
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
}

void imgOperatorWindows::buildShitForThisImgpath(QString imgPath)
{
<<<<<<< HEAD
//    buildFlirImgObjAndSegmentorObj(imgPath);

//    fW->initFlirImgWindow(fimg);
//    sW->initSegmentorWindow(segmentor);

//    connectEverything();
=======
    buildFlirImgObjAndSegmentorObj(imgPath);

    fW->readImage(fimg);
    sW->readImage(segmentor);

    connectEverything();
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
}

void imgOperatorWindows::buildFlirImgObjAndSegmentorObj(QString imgPath)
{
<<<<<<< HEAD
//    fimg->processImage(imgPath.toStdString());
//    //fimg->loadPreorderedRAWsImage();

//    flirImgMetadata *fmd;
//    fmd = fimg->getMetadata();
//    // fmd->printReadMetadata();
//    // fmd->printCalcedMetadata();

//    if(fimg->didUnorderedRAWsExtractionSucceed()){
//        // fimg->printImageSummary();
//        // cout<<"Began processing..." << endl;
//        fimg->calcTempForEveryPixel();
//        // cout<<"End of processing..." << endl;
//    }
//    else {
//        qDebug() << "Thermal conversion failed";
//        std::exit(1);
//    }

//    qDebug() << " imgOperatorWindows  : buildSegmentorObj() - Beginning with the segmentor object ";
//    std::string thermimgpath = fimg->getDisplayRAWsImgpath();
//    qDebug() << " imgOperatorWindows  : buildSegmentorObj() - About to build new segmentor object ";
//    segmentor->loadDisplayRAWsImage(thermimgpath, cv::IMREAD_COLOR);
//    qDebug() << " tgfsTabbed Window : buildSegmentorObj() - About to segment it ";
//    segmentor->segmentImage();
//    qDebug() << " tgfsTabbed Window : buildSegmentorObj() - Done ";
}


void imgOperatorWindows::isPointKeBlobKaTempNikaaloMadarchod(QPoint p)
{
//    // qDebug() <<" imgOperatorWindows::isPointKeBlobKaTempNikaaloMadarchod -"
//    //          <<" Inside madarchod blob nikaalo " << p.x() << ", " << p.y();
//    std::vector<Point2i> pts;
//    pts = segmentor->getIsPointKaBlob(p);
//    std::cout << " Num of points in this blob " << pts.size() << std::endl;
//    double avgBlobTemp;
//    avgBlobTemp = fimg->calcBlobAvgTemp(pts);
//    emit blobAvgTempLeleBhosdike(avgBlobTemp);
//    // std::cout << "Avg blob Temp " << avgBlobTemp << endl;
//    // std::cout << "Blob num " << blobNum << std::endl;
=======
    fimg->loadOrigImage(imgPath.toStdString());
    fimg->loadRAWThermalImage();

    flirImgMetadata *fmd;
    fmd = fimg->getMetadata();
    // fmd->printReadMetadata();
    // fmd->printCalcedMetadata();

    if(fimg->didThermalConversionSucceed()){
        // fimg->printImageSummary();
        // cout<<"Began processing..." << endl;
        fimg->calcTempForEveryPixel();
        // cout<<"End of processing..." << endl;
    }
    else {
        qDebug() << "Thermal conversion failed";
        std::exit(1);
    }

    qDebug() << " imgOperatorWindows  : buildSegmentorObj() - Beginning with the segmentor object ";
    std::string thermimgpath = fimg->getThermalImgPath();
    qDebug() << " imgOperatorWindows  : buildSegmentorObj() - About to build new segmentor object ";
    segmentor->loadThermalImage(thermimgpath, cv::IMREAD_COLOR);
    qDebug() << " tgfsTabbed Window : buildSegmentorObj() - About to segment it ";
    segmentor->segmentImage();
    qDebug() << " tgfsTabbed Window : buildSegmentorObj() - Done ";
}


void imgOperatorWindows::isPointKeBlobKaTempNikaaloMadarchod(QPointF p)
{
    // qDebug() <<" imgOperatorWindows::isPointKeBlobKaTempNikaaloMadarchod -"
    //          <<" Inside madarchod blob nikaalo " << p.x() << ", " << p.y();
    std::vector<Point2i> pts;
    pts = segmentor->getIsPointKaBlob(p);
    std::cout << " Num of points in this blob " << pts.size() << std::endl;
    double avgBlobTemp;
    avgBlobTemp = fimg->calcBlobAvgTemp(pts);
    emit blobAvgTempLeleBhosdike(avgBlobTemp);
    // std::cout << "Avg blob Temp " << avgBlobTemp << endl;
    // std::cout << "Blob num " << blobNum << std::endl;
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
}


void imgOperatorWindows::connectEverything()
{
    connect(fW,   SIGNAL(leafDetectButtonToggled(bool)),
            this, SLOT(itsTimeToDisplayTheBlobOverlaidImage(bool)));
    // connect(fW, SIGNAL(plssBlobBataaoIsPointKa(QPointF)),
    //         this, SLOT(isPointKeBlobKaTempNikaaloMadarchod(QPointF)));
    // connect(this, SIGNAL(blobAvgTempLeleBhosdike(double)),
    //         fW, SLOT(emitBlobAvgTempMilaBhosadiwaale(double)));
}

void imgOperatorWindows::emitBlobAvgTempMilaBhosadiwaale(double)
{

}


void imgOperatorWindows::itsTimeToDisplayTheBlobOverlaidImage(bool y)
{
<<<<<<< HEAD
//    // cv::Mat overlayMat = sW->getOverlayImage();

//    qDebug() << " imgOperatorWindows : displayOverlaidImageForOneImage() - inside the decider";
//    if(y){
//      qDebug() << " imgOperatorWindows : displayOverlaidImageForOneImage() - true overlaid image ";
//      fW->updateOverlaidImage(sW->getOverlayImage());
//    }
//    else {
//      qDebug() << " imgOperatorWindows : displayOverlaidImageForOneImage() - restore original image";
//      //cout << " false overlaid image " << endl;
//      fW->restoreOriginalImage();
//    }
=======
    // cv::Mat overlayMat = sW->getOverlayImage();

    qDebug() << " imgOperatorWindows : displayOverlaidImageForOneImage() - inside the decider";
    if(y){
      qDebug() << " imgOperatorWindows : displayOverlaidImageForOneImage() - true overlaid image ";
      fW->updateOverlaidImage(sW->getOverlayImage());
    }
    else {
      qDebug() << " imgOperatorWindows : displayOverlaidImageForOneImage() - restore original image";
      //cout << " false overlaid image " << endl;
      fW->restoreOriginalImage();
    }
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
}
