#ifndef FLIRTEMP2_H
#define FLIRTEMP2_H

#include <QObject>
#include "opencv2/opencv.hpp"
#include "flirtags.h"
#include <QDebug>


struct f1data
{
    unsigned short raw;
    double rawobjv;
    double t;

    int rowNum, colNum;
};

struct f2data
{
    unsigned short raw;
    int raw2;
    double rawobjv;
    double t;

    int rowNum, colNum;
};

// used for debugging mainly
struct f3data
{
    unsigned short raw;
    double rawobjv1;
    double t1;

    int raw2;
    double rawobjv2;
    double t2;

    int rowNum, colNum;

    string debugstring()
    {
        std::stringstream r,c,d1,d2,out;
        r << "(" << rowNum << ", ";
        c << colNum << ")";
        d1 << "(raw,  t1)" << setw(6) << raw  << setw(8) << t1;
        d2 << "(raw2, t2)" << setw(6) << raw2 << setw(8) << t2;

        out << std::left << std::setw(6) << r.str()
            << std::right << std::setw(4) << c.str()
            << " : "
            << d1.str()
            << endl
            << std::right << setw(37) << d2.str()
            << endl;
        return out.str();
    }
};

enum class formulaType
{
    formula1, formula2, formula3
};

struct pixData
{
    formulaType type;
    union {
        f1data f1;
        f2data f2;
        f3data f3;
    };
};

class flirtemp2 : public QObject
{
    Q_OBJECT
public:
    explicit flirtemp2(flirtags* ftags, const cv::Mat raws_mat);
    void calc_temp();
    void debug_temps();
    int get_raw_at(int,int);
    double get_temp_at(int,int);
private:

    void formula1(int,int,f1data&);
    void formula2(int,int,f2data&);
    void formula3(int,int,f3data&);
    void init3(f3data&);
    double calc_rawobjv(int);
    double calc_temp_from_rawobjv(double);
    int change_byteorder(unsigned short);
private:
    cv::Mat raws_m;
    std::vector<pixData> pixels;
    std::vector<std::vector<pixData>> pixels2;
    bool isReordered;
    flirtags *ft;
    int w,h;
signals:
    void signal_flirtemp_progress(int);
};

#endif // FLIRTEMP2_H
