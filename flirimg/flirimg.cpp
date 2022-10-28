
#include "flirimg.h"
#include <QDebug>
#include <QProcess>
using namespace cv;


// at the end of processImage, i should have the following Mats
// displayOrigImgMat        -- the original image read-in in colour
//                          -- used for display purposes, therefore encoded
// unorderedRAWsMat         -- RAW sensor values only used internally
//                          -- not used for display anywhere
// displayUnorderedRAWsMat  -- RAW sensor values, used for repaletting
//                          -- used for the final repaletted display
// displayOrderedRAWsMat    -- RAW sensor values, used for repaletting
//                          -- used for the final repaletted display
// repalettedImgMat         -- the colorized one
void flirImg::processImage(std::string fimgpath)
{
  initializePaths(fimgpath);
  //readFileIntoMat(imgpath, origImgMat);
  readFileIntoMat(imgpath, origImgMat, cv::IMREAD_COLOR);
  extractAllTags();
  calcTminTmax();
  printImageSummary();
  if(extractRAWs()){
    readFileIntoMat(unorderedRAWs_imgpath, unorderedRAWsMat, cv::IMREAD_UNCHANGED);
    if(RawThermalImageType == "PNG"){
        qDebug() << "flirImg::processImage -- PNG";
        changeByteOrderOfRAWsMat();
    }
    else {
        qDebug() << "flirImg:: not a png but modern image";
        changeByteOrderOfRAWsMat();
    }
    calcTempForEveryPixel();
    normalizeTempMat();
    generateGrayscaleImage();
    applyPalette();
//    if(extractPalette()){

//    }
//    if(extractEmbeddedRGBImage()){

//    }
    segmentor = new Segmentor();
    segmentor->initSegmentor(getRepalettedImgMat().clone());
    segmentor->segmentImage();
    // writeRaw2TempValues();

//    displayUsingOpenCV("grayscaleTherm", grayscaleThermalDisplayMat);
//    displayUsingOpenCV("repaletted", repalettedImgMat);

  } else {
      qDebug() << "flirImg::processImage -- Couldn't extract unordered RAWs. Exiting";
      exit;
  }
}


void flirImg::generateGrayscaleImage()
{
    grayscaleThermalDisplayMat = Mat::zeros(normalizedThermalMat.size(), CV_8UC1);
    for(int colNum=0; colNum<rawthermalimagewidth; colNum++){
        for(int rowNum=0; rowNum<rawthermalimageheight; rowNum++){
            double         g1 = normalizedThermalMat.at<double>(rowNum, colNum);
                           g1 = g1 * 255;
            uchar g2          = (uchar)(g1);
            grayscaleThermalDisplayMat.at<uchar>(rowNum, colNum) = g2;
        }
    }
}


void flirImg::normalizeTempMat()
{
    Mat srcMat                = temperatures.clone();
    double                       t, final_t;
    double                      norm_t;
    normalizedThermalMat = Mat::zeros(srcMat.size(), CV_64FC1);
    double minval, maxval;
    minMaxIdx(srcMat, &minval, &maxval);
    for(int colNum=0; colNum<rawthermalimagewidth; colNum++){
        for(int rowNum=0; rowNum<rawthermalimageheight; rowNum++){
            t                    = srcMat.at<double>(rowNum, colNum);
            norm_t               = (double)(t - minval)/(double)(maxval - minval);
            normalizedThermalMat.at<double>(rowNum, colNum) = norm_t;
            //qDebug() << "t : " << t << " norm_t : " << norm_t;
        }
        //qDebug() << "Normalized Temp " << norm_t;
    }
}

// map the (lower, upper) bounds to the
// raw value range
// then return a matrix with only those pixels
// that lie between these two values
Mat flirImg::getRangedMat(int lower, int upper, bool isRepalettedMode)
{
    // rawmin corresponds to 0
    // rawmax corresponds to 100
    // lower-upper corresponds to what values between rawmin-rawmax ?
    int    minRawSelected, maxRawSelected;
    double grayRawMax, grayRawMin, grayRawDelta;
    //rawDelta = rawMax - rawMin;
    minMaxIdx(unorderedRAWsMat, &grayRawMin, &grayRawMax);
    grayRawDelta = grayRawMax - grayRawMin;
    minRawSelected = rawMin + ((lower * grayRawDelta)/100);
    maxRawSelected = rawMin + ((upper * grayRawDelta)/100);
    qDebug() << "flirImg::getRangedMat() - lowerBound : " << lower << " upperBound " << upper;
    qDebug() << "flirImg::getRangedMat() - minRawSelected : " << minRawSelected << "maxRawSelected" << maxRawSelected;
    int validPixCount = 0;

    rangedGrayscaleThermalDisplayMat = Mat::zeros(grayscaleThermalDisplayMat.size(), CV_8UC1);
    rangedGrayscaleThermalDisplayMat = grayscaleThermalDisplayMat.clone();
    rangedRepalettedThermalDisplayMat= Mat::zeros(repalettedImgMat.size(), CV_8UC3);
    rangedRepalettedThermalDisplayMat= repalettedImgMat.clone();
    for(int colNum=0; colNum < rawthermalimagewidth; colNum++){
        for(int rowNum=0; rowNum < rawthermalimageheight; rowNum++){
            unsigned short v = unorderedRAWsMat.at<unsigned short>(rowNum, colNum);
            //qDebug() << "Testing " << v << " against " << minRawSelected << " and " << maxRawSelected;
            if(v >= minRawSelected && v <= maxRawSelected){
                //rangedGrayscaleThermalDisplayMat.at<uchar>(rowNum, colNum) = grayscaleThermalDisplayMat.at<uchar>(rowNum, colNum);
                validPixCount += 1;
            }
            else{
                if(isRepalettedMode)
                    rangedRepalettedThermalDisplayMat.at<Vec3b>(rowNum, colNum) = Vec3b(255,255,255);
                else
                    rangedGrayscaleThermalDisplayMat.at<uchar>(rowNum, colNum) = 255;
            }
        }
    }

    qDebug() << "flirImgFrame::getRangedMat() -- Valid Pix Count " << validPixCount;
    if(isRepalettedMode)
        return rangedRepalettedThermalDisplayMat;
    else
        return rangedGrayscaleThermalDisplayMat;
}

void flirImg::getNormalizedRAWMinMax(int &rmin, int &rmax)
{
    double a, b;
    minMaxIdx(grayscaleThermalDisplayMat, &a, &b);
    rmin = (int)a;
    rmax = (int)b;
}

Mat flirImg::getTransparencyOverlaidMat(double overlay_alpha)
{
    overlaidMat      = Mat::zeros(grayscaleThermalDisplayMat.size(), CV_8UC3);
    Mat srcMat       = Mat::zeros(grayscaleThermalDisplayMat.size(), CV_8UC3);
    cv::cvtColor(grayscaleThermalDisplayMat, srcMat, cv::COLOR_GRAY2BGR);
    Mat overlay      = segmentor->getOverlayMat();

    cv::addWeighted(srcMat,    1.0 - overlay_alpha,
                    overlay,    overlay_alpha,
                    0.0,
                    overlaidMat);
    qDebug() << "flirImg::getTransparencyOverlaidMat -- Done with the addition";
    return overlaidMat;
}

#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
void flirImg::applyPalette()
{
    repalettedImgMat = Mat::zeros(normalizedThermalMat.size(), CV_8UC3);
    applyColorMap(grayscaleThermalDisplayMat, repalettedImgMat, COLORMAP_INFERNO);
}

bool flirImg::didDisplayRAWsConversionSucceed()
{

}

void flirImg::changePalette(ColormapTypes ctype)
{
    applyColorMap(grayscaleThermalDisplayMat, repalettedImgMat, ctype);
}

QString flirImg::getStatusString(int rowNum, int colNum)
{
    QString rowString           = QString::number(rowNum).leftJustified(4);
    QString colString           = QString::number(colNum).leftJustified(4);
    // qDebug() << "flirImg::getStatusString #1";
    QString orderedRAWvalString = QString::number(orderedRAWsMat.at<unsigned short>(rowNum, colNum)).leftJustified(6);
    // qDebug() << "flirImg::getStatusString #2";
    QString tempString          = QString::number(temperatures.at<double>(rowNum, colNum)).leftJustified(6);
    // qDebug() << "flirImg::getStatusString #3";
    // QString debugString         = QString::number(displayUnorderedRAWsMat.at<unsigned short>(rowNum, colNum)).leftJustified(6);
    // qDebug() << "flirImg::getStatusString #4";

    QString statusString = rowString
                           + ", "
                           + colString
                           + " : "
                           + orderedRAWvalString
                           + " - "
                           + tempString;
    //                     + " - "
    //                     + debugString;

    return statusString;
}

QString flirImg::getSegmentorStatusString(QPoint imgPos)
{
    int colNum = imgPos.x();
    int rowNum = imgPos.y();

    int    blobID      = segmentor->getIsPointKaBlobID(imgPos);
    int    blobArea    = segmentor->getIsPointKaBlobArea(imgPos);
    int    numOfBlobs  = segmentor->getNumOfBlobs();
    //double blobAvgTemp = getIsPointKeBlobKaAvgTemp(imgPos);

    // this should cause a chain of signals/slots to update
    // the blob avg temparature of this place
    QString statusString = QString::number(colNum)
                           + ", "
                           + QString::number(rowNum)
                           + " : Blob ID - "
                           + QString::number(blobID)
                           + " : Num of Blobs - "
                           + QString::number(numOfBlobs)
                           + " : Blob Area - "
                           + QString::number(blobArea);
//                           + " : Blob temp "
//                           + QString::number(blobAvgTemp);
    return statusString;

}

#include <unistd.h>
void flirImg::displayUsingOpenCV(std::string window_name, Mat m)
{
    // child process
    namedWindow(window_name, WINDOW_NORMAL);
    setWindowProperty(window_name, WND_PROP_AUTOSIZE, WINDOW_AUTOSIZE);
    imshow(window_name, m);
}

double flirImg::getPixelTemperature(int rowNum, int colNum)
{
    return temperatures.at<double>(rowNum, colNum);
}

unsigned short flirImg::getOrderedRAWval(int rowNum, int colNum)
{
    return orderedRAWsMat.at<unsigned short>(rowNum, colNum);
}

void flirImg::initializePaths(std::string fimgpath)
{
    imgpath                         = QString::fromStdString(fimgpath);
    filename                        = imgpath.split('.').at(0);
    unorderedRAWs_imgpath           = QString(filename + ".unordered.raws.pgm");
    orderedRAWs_imgpath             = QString(filename + ".ordered.raws.pgm");
    displayOrderedRAWs_imgpath      = QString(filename + ".displayOrderedRAWs.png");
    displayUnorderedRAWs_imgpath    = QString(filename + ".displayUnorderedRAWs.png");
    embeddedRGB_imgpath             = QString(filename + ".embeddedrgb.png");
    linearGrayscaleDisplay_imgpath  = QString(filename + ".linear.png");
    repalettedOrderedRAWs_imgpath   = QString(filename + ".repalettedOrderedRAWs.png");
    repalettedUnorderedRAWs_imgpath = QString(filename + ".repalettedUnorderedRAWs.png");

    tempsCSVFile                    = QString(filename + ".raw2temps_rwaala.csv");
    unorderedRAWsCSVFile            = QString(filename + ".unorderedRAWs.csv");
    orderedRAWsCSVFile              = QString(filename + ".orderedRAWs.csv");

    rawpalette_imgpath              = QString(filename + ".rawpalette.png");
    palette_imgpath                 = QString(filename + ".pal.png");

    metadataFile                    = QString(filename + ".metadata.txt");
}

flirImg::flirImg()
{
  imgpath = "";
  orderedRAWs_imgpath ="";
  emsgs   = new ErrMsgs();
  temperatures = NULL;
}

flirImg::flirImg(QString imgPath)
{
    flirImg();
    processImage(imgPath.toStdString());
}
