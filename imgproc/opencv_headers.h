#ifndef OPENCV_HEADERS_H
#define OPENCV_HEADERS_H
#include <QObject>

#ifdef Q_OS_WIN32
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif

#ifdef Q_OS_LINUX
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif


#endif // OPENCV_HEADERS_H
