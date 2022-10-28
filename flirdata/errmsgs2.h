#ifndef ERRMSGS2_H
#define ERRMSGS2_H

#include <QObject>
#include <iostream>
#include <string>
// #include "opencv_headers.h"
using namespace std;
//using namespace cv;


class errmsgs2 : public QObject
{
    Q_OBJECT
public:
    explicit errmsgs2(QObject *parent = nullptr);
public:
    std::string getRGBimgErrorMsg();
    std::string getThermalImgErrorMsg();
    std::string getPHPimgErrorMsg();
signals:

};

#endif // ERRMSGS_H
