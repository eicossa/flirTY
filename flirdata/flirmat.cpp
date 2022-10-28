#include <QDebug>
#include <QImage>
#include <QPixmap>

#include "flirmat.h"
#include "paths.h"
#include "utilfuncs.h"
using namespace utilfuncs;
using namespace envt;
using namespace writers;

flirmat::flirmat(flirtags* ftags)
{
    ft = ftags;
    QString raws1_path = ft->p->get_raws1_dpath();
    extract_raws1(raws1_path);
    utilfuncs::read_file_to_mat(raws1_path, raws1, cv::IMREAD_UNCHANGED);

    ftemp2 = new flirtemp2(ft, raws1);
    ftemp2->debug_temps();

    // render_temperatures_grayscale(ftemp->temps(), temps_gray);
    // apply_palette(cv::COLORMAP_INFERNO);
}

void flirmat::extract_raws1(QString imgpath)
{
    // bool f = extract_raws_cmd(paths::get_exiftool_path());
    QString raws1_result, raws1_errors;

    if(utilfuncs::doesFileExist(ft->p->get_raws1_dpath())) {
        qDebug() << ft->p->get_raws1_dpath() << " already extracted. Doing nothing. ";
    }
    else {
        utilfuncs::execute_cmd(envt::get_exiftool_path(),
                           ft->p->exiftool_extract_args(RAWS1),
                           raws1_result, raws1_errors,
                           execute);


        if(utilfuncs::doesFileExist(ft->p->get_raws1_dpath())) {
            qDebug() << "... flirdata::extract_raws success.";
        }
        else {
            qDebug() << "... flirdata::extract_raws failed. Exiting";
            exit;
        }
    }
}



//bool flirmat::extract_embedded_rgb()
//{
//  // Extract embedded image
//  // exiftool FLIR1097.jpg -b -EmbeddedImage FLIR1097.rgb.png
//    return true;
//}


void flirmat::render_temperatures_grayscale(const cv::Mat temps1, cv::Mat &temps_gray1)
{
    normalize_mat(temps1, temps_norm);
    grayscale_mat(temps_norm, temps_gray1);
}

void flirmat::normalize_mat(const cv::Mat inputMat, cv::Mat &outputMat)
{
    double t, final_t;
    double norm_t;
    outputMat = Mat::zeros(inputMat.size(), CV_64FC1);
    // n_temp_mat = Mat::zeros(srcMat.size(), CV_64FC1);
    double minval, maxval;
    minMaxIdx(inputMat, &minval, &maxval);

    int w = inputMat.size().width;
    int h = inputMat.size().height;

    for(int colNum=0; colNum < w; colNum++){
        for(int rowNum=0; rowNum < h; rowNum++){
            t                    = inputMat.at<double>(rowNum, colNum);
            norm_t               = (double)(t - minval)/(double)(maxval - minval);
            outputMat.at<double>(rowNum, colNum) = norm_t;
            //qDebug() << "t : " << t << " norm_t : " << norm_t;
        }
        //qDebug() << "Normalized Temp " << norm_t;
    }
}

// convert a double Mat to a grayscale int Mat
void flirmat::grayscale_mat(const cv::Mat inputMat, cv::Mat &outputMat)
{
    outputMat = Mat::zeros(inputMat.size(), CV_8UC1);
    const int w = inputMat.size().width;
    const int h = inputMat.size().height;
    double g1;
    uchar  g2;
    for(int colNum=0; colNum<w; colNum++){
        for(int rowNum=0; rowNum<h; rowNum++){
            g1 = inputMat.at<double>(rowNum, colNum);
            g1 = g1 * 255;
            g2 = (uchar)(g1);
            outputMat.at<uchar>(rowNum, colNum) = g2;
        }
    }
}

unsigned short flirmat::change_pixel_byteorder(unsigned short wrong_byte_order)
{
    unsigned short x, y, x1, x2, x3;
    unsigned short reordered_val;
    x               = wrong_byte_order;
    x1              = x >> 8;
    x2              = (x & 0x0ff);
    x3              = x2 << 8;
    y               = x1 + x3;
    reordered_val = y;
    return reordered_val;
}

#include <inttypes.h>
//#include <arpa/inet.h>

void flirmat::change_mat_byteorder(const cv::Mat inputMat, cv::Mat &outputMat)
{
    unsigned short x, y, x1, x2, x3, unordered_val, reordered_val;
    int width  = inputMat.size().width;
    int height = inputMat.size().height;

    outputMat = Mat::zeros(height, width, CV_16U);

    for(int i=0; i < height; i++){
      for(int j=0; j < width; j++){
          unordered_val = inputMat.at<unsigned short>(i, j);
          reordered_val = change_pixel_byteorder(unordered_val);

          outputMat.at<int>(i, j) = reordered_val;
          //don't use int here ... it crashes at runtime

       }
    }
}

// map the (lower, upper) bounds to the
// raw value range
// then return a matrix with only those pixels
// that lie between these two values
Mat flirmat::get_ranged_mat(int lower, int upper, bool isRepalettedMode)
{
    // rawmin corresponds to 0
    // rawmax corresponds to 100
    // lower-upper corresponds to what values between rawmin-rawmax ?
    int    minRawSelected, maxRawSelected;
    double grayRawMax, grayRawMin, grayRawDelta;
    //rawDelta = rawMax - rawMin;
    minMaxIdx(raws, &grayRawMin, &grayRawMax);
    grayRawDelta = grayRawMax - grayRawMin;
    minRawSelected = ft->rawMin + ((lower * grayRawDelta)/100);
    maxRawSelected = ft->rawMin + ((upper * grayRawDelta)/100);
    qDebug() << "flirImg::getRangedMat() - lowerBound : " << lower << " upperBound " << upper;
    qDebug() << "flirImg::getRangedMat() - minRawSelected : " << minRawSelected << "maxRawSelected" << maxRawSelected;
    int validPixCount = 0;

    ranged = temps_gray.clone();
    const int w = raws.size().width;
    const int h = raws.size().height;

    for(int colNum=0; colNum < w; colNum++){
        for(int rowNum=0; rowNum < h; rowNum++){
            unsigned short v = raws.at<unsigned short>(rowNum, colNum);
            //qDebug() << "Testing " << v << " against " << minRawSelected << " and " << maxRawSelected;
            if(v >= minRawSelected && v <= maxRawSelected){
                //rangedGrayscaleThermalDisplayMat.at<uchar>(rowNum, colNum) = grayscaleThermalDisplayMat.at<uchar>(rowNum, colNum);
                validPixCount += 1;
            }
            else{
                if(isRepalettedMode)
                    ranged.at<Vec3b>(rowNum, colNum) = Vec3b(255,255,255);
                else
                    ranged.at<uchar>(rowNum, colNum) = 255;
            }
        }
    }

    return ranged;

}

//void flirmat::get_NormalizedRAWMinMax(int &rmin, int &rmax)
//{
//    double a, b;
//    minMaxIdx(temps_gray, &a, &b);
//    rmin = (int)a;
//    rmax = (int)b;
//}

// apply an overlay with the specified transparency level
//void flirmat::apply_overlay(double overlay_alpha)
//{
//    cv::addWeighted(display,    1.0 - overlay_alpha,
//                    overlay,    overlay_alpha,
//                    0.0,
//                    display2);
//    display = display2;
//}

#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
//void flirmat::apply_palette(ColormapTypes ctype)
//{
//    display = Mat::zeros(temps_gray.size(), CV_8UC3);
//    applyColorMap(temps_gray, display, ctype);
//}





QString flirmat::get_status_string(int rowNum, int colNum)
{
    QString row = QString::number(rowNum).leftJustified(4);
    QString col = QString::number(colNum).leftJustified(4);
    QString rowcol_s = row + ", " + col + " : ";

    // QString raw_s = QString::number(raws.at<unsigned short>(rowNum, colNum)).leftJustified(6);
    QString raw_s = QString::number(ftemp2->get_raw_at(rowNum, colNum));
    if(isReordered) {
        unsigned short r = raws1.at<unsigned short>(rowNum,colNum);
        raw_s += " ( " + QString::number(r).leftJustified(6) + " ) ";
    }

    // QString temp_s = QString::number(temps1.at<double>(rowNum, colNum)).leftJustified(6);
    QString temp_s = QString::number(ftemp2->get_temp_at(rowNum, colNum)).leftJustified(6);

    // qDebug() << "flirImg::getStatusString #3";
    // QString debugString         = QString::number(displayUnorderedRAWsMat.at<unsigned short>(rowNum, colNum)).leftJustified(6);
    // qDebug() << "flirImg::getStatusString #4";


    QString status_s = rowcol_s + raw_s + temp_s;
    return status_s;
}

void flirmat::dump_mats()
{
    writers::write_mat_to_csv(ft->p->get_raws1_dpath(), raws1);
    writers::write_mat_to_csv(ft->p->get_raws2_dpath(), raws2);
    writers::write_mat_to_csv(ft->p->get_temp1_csv_dpath(), temps1);
    writers::write_mat_to_csv(ft->p->get_temp2_csv_dpath(), temps2);

    writers::write_mat(ft->p->get_raws1_dpath(), raws1);
}

//QPixmap flirmat::qimage()
//{
//    QImage::Format imgType;
//    cv::Mat imgMat = display.clone();
//    if(imgMat.type() == 16)
//        imgType = QImage::Format_RGB888;
//    if(imgMat.type() == 0)
//        imgType = QImage::Format_Grayscale8;
//    QImage img = QImage(imgMat.data,
//                        imgMat.cols, imgMat.rows,
//                        imgMat.step,
//                        imgType);
//    QPixmap mPixmap = QPixmap::fromImage(img.rgbSwapped());
//    return mPixmap;
//}
