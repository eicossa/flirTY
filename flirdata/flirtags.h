#ifndef FLIRTAGS_H
#define FLIRTAGS_H

#include <QObject>
#include <QMap>
#include "errmsgs2.h"
#include "enumtypes.h"
#include "paths.h"
#include "opencv2/opencv.hpp"


class flirtags : public QObject
{
    Q_OBJECT
public:
    flirtags(QString);
signals:

public:
    void read_metadata();
    void calc_metadata();
    void read_thermal_data(QString);

    void img_summ5();
public:
    paths*                 p;
private:
    bool read_file_to_mat(QString, cv::Mat&, cv::ImreadModes);
    void extract_tags(QString);
    bool extract_raws(QString);
    void calc_rmin_rmax();
    void print_img_summary();
    void print_img_summary2();
public:
    // read thermal metadata
    double             planckr1, planckb, planckf, plancko, planckr2;
    double             tref, emis;
    double             ata1, ata2, atb1, atb2, atx;
    double             atmTemp, IRWTemp, IRWTrans, relHum;
    double             rawvaluerange, rawvaluemedian;
    double             focusDist, objDist;
    int                paletteColors; // palette stuff
    std::string        palName; // palette name
    QString            filename;
    // read basic metadata
    int                imagewidth, imageheight;
    int                rawthermalimagewidth, rawthermalimageheight;
    // camera details
    std::string        RawThermalImageType;
    std::string        cmodel;
    // calculated values
    double             rawMin, rawMax, rawRefl;
    double             rawMinObj, rawMaxObj;
    double             tmin, tmax;
private:
    QMap<QString, QString> flirTagsMap;
    errmsgs2*               emsgs;

private:
    void               execute_cmd(QString, QStringList, QString&, QString&, cmdExecType);
};

#endif // FLIRTAGS_H
