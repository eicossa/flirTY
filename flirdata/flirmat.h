#ifndef FLIRMAT_H
#define FLIRMAT_H

#include <QObject>
#include "opencv2/opencv.hpp"
#include "flirtemp2.h"
#include "flirtags.h"

using namespace cv;

class flirmat : public QObject
{
    Q_OBJECT
public:
    flirmat(flirtags* ft);
    void display_opencv();
    // void read_thermal_data(QString imgpath);
    void apply_overlay(double overlay_alpha);
    double get_temp_at(int rowNum, int colNum)
    {
        return temps1.at<double>(rowNum, colNum);
    }

    unsigned short get_raw_at(int rowNum, int colNum)
    {
        return raws.at<unsigned short>(rowNum, colNum);
    }
    QString get_status_string(int,int);
    cv::Mat get_ranged_mat(int, int, bool);
    QPixmap qimage();
    void dump_mats();
public slots:
private:
    // Step1 : extract raw thermal data
    void extract_raws1(QString imgpath);
    void create_raws_mat();
    // Step2 : change byteorder if necessary
    void change_mat_byteorder(const cv::Mat inputMat, cv::Mat &outputMat);
    unsigned short change_pixel_byteorder(unsigned short wrong_byte_order);
    // Step3 : temperatures are calculated by flirtemp object ftemp
    // Step4 : convert temperatures mat into a format suitable for display
    void render_temperatures_grayscale(const cv::Mat temps, cv::Mat &temps_gray);
    void normalize_mat(const cv::Mat inputMat, cv::Mat &outputMat);
    void grayscale_mat(const cv::Mat inputMat, cv::Mat &outputMat);
    // Step5 : apply a palette to the image to colorize it
    // void apply_palette(ColormapTypes c);
public:
    flirtemp2 *ftemp2;

private:
    flirtags* ft;
    // flirtemp *ftemp;
    // flirtemp2 *ftemp2;
    // flirtemp *ftemp1, *ftemp2;
    cv::Mat raws, raws1, raws2; // all unsigned short
    cv::Mat temps1, temps2;      // double
    cv::Mat temps_norm; // float ? or double ?
    cv::Mat temps_gray; // int ?
    cv::Mat ranged; // to return masks or partial Mats based on some criterion of QPoints
    bool isReordered; // did we do reordering for the raws
};

#endif // FLIRMAT_H
