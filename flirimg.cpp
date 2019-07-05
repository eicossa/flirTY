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
    calcTempForEveryPixel();
    normalizeTempMat();
    generateGrayscaleImage();
    applyPalette();
    if(extractPalette()){

    }
    if(extractEmbeddedRGBImage()){

    }
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
    grayscaleThermalDisplayMat = Mat::zeros(normalizedThermalDisplayMat.size(), CV_8UC1);
    for(int colNum=0; colNum<rawthermalimagewidth; colNum++){
        for(int rowNum=0; rowNum<rawthermalimageheight; rowNum++){
            double         g1 = normalizedThermalDisplayMat.at<double>(rowNum, colNum);
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
    normalizedThermalDisplayMat = Mat::zeros(srcMat.size(), CV_64FC1);
    double minval, maxval;
    minMaxIdx(srcMat, &minval, &maxval);
    for(int colNum=0; colNum<rawthermalimagewidth; colNum++){
        for(int rowNum=0; rowNum<rawthermalimageheight; rowNum++){
            t                    = srcMat.at<double>(rowNum, colNum);
            norm_t               = (double)(t - minval)/(double)(maxval - minval);
            normalizedThermalDisplayMat.at<double>(rowNum, colNum) = norm_t;
            //qDebug() << "t : " << t << " norm_t : " << norm_t;
        }
        //qDebug() << "Normalized Temp " << norm_t;
    }


}


Mat flirImg::getTransparencyOverlaidMat(double overlay_alpha)
{
    overlaidMat      = Mat::zeros(grayscaleThermalDisplayMat.size(), CV_8UC3);
    Mat overlay      = segmentor->getOverlayMat();

    qDebug() << "flirImg::getTransparencyOverlaidMat -- Size of overlay" << overlay.size().width << " " << overlay.size().height;
    qDebug() << "flirImg::getTransparencyOverlaidMat -- Size of overlaid " << overlaidMat.size().width << " " << overlaidMat.size().height;

    cv::addWeighted(overlaidMat,    1.0 - overlay_alpha,
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
    repalettedImgMat = Mat::zeros(normalizedThermalDisplayMat.size(), CV_8UC3);

    //    Mat repalettedThermalDisplayMat_rainbow;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_rainbow, COLORMAP_RAINBOW);
    //    displayUsingOpenCV("rainbow", repalettedThermalDisplayMat_rainbow);

    //    Mat repalettedThermalDisplayMat_autumn;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_autumn, COLORMAP_AUTUMN);
    //    displayUsingOpenCV("autumn", repalettedThermalDisplayMat_autumn);

    //    Mat repalettedThermalDisplayMat_bone;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_bone, COLORMAP_BONE);
    //    displayUsingOpenCV("bone", repalettedThermalDisplayMat_bone);

    //    Mat repalettedThermalDisplayMat_cool;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_cool, COLORMAP_COOL);
    //    displayUsingOpenCV("cool", repalettedThermalDisplayMat_cool);

    //    Mat repalettedThermalDisplayMat_hot;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_hot, COLORMAP_HOT);
    //    displayUsingOpenCV("hot", repalettedThermalDisplayMat_hot);

    //    Mat repalettedThermalDisplayMat_hsv;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_hsv, COLORMAP_HSV);
    //    displayUsingOpenCV("hsv", repalettedThermalDisplayMat_hsv);

    //    Mat repalettedThermalDisplayMat_jet;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_jet, COLORMAP_JET);
    //    displayUsingOpenCV("jet", repalettedThermalDisplayMat_jet);

    //    Mat repalettedThermalDisplayMat_ocean;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_ocean, COLORMAP_OCEAN);
    //    displayUsingOpenCV("ocean", repalettedThermalDisplayMat_ocean);

    //    Mat repalettedThermalDisplayMat_parula;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_parula, COLORMAP_PARULA);
    //    displayUsingOpenCV("parula", repalettedThermalDisplayMat_parula);

    //    Mat repalettedThermalDisplayMat_pink;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_pink, COLORMAP_PINK);
    //    displayUsingOpenCV("pink", repalettedThermalDisplayMat_pink);

    //    Mat repalettedThermalDisplayMat_spring;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_spring, COLORMAP_SPRING);
    //    displayUsingOpenCV("spring", repalettedThermalDisplayMat_spring);

    //    Mat repalettedThermalDisplayMat_summer;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_summer, COLORMAP_SUMMER);
    //    displayUsingOpenCV("summer", repalettedThermalDisplayMat_summer);

    //    Mat repalettedThermalDisplayMat_winter;
    //    applyColorMap(grayscaleThermalDisplayMat, repalettedThermalDisplayMat_winter, COLORMAP_WINTER);
    //    displayUsingOpenCV("winter", repalettedThermalDisplayMat_winter);

    applyColorMap(grayscaleThermalDisplayMat, repalettedImgMat, COLORMAP_INFERNO);
    //displayUsingOpenCV("inferno", repalettedImgMat);


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

int flirImg::getOrderedRAWval(int rowNum, int colNum)
{
    return orderedRAWsMat.at<int>(rowNum, colNum);
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
