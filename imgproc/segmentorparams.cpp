#include "segmentorparams.h"
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

SegmentorParams::SegmentorParams()
{
    binaryThreshLoVal = 40;
    binaryThreshMaxVal = 255;
    binaryThreshFlags = THRESH_BINARY | THRESH_OTSU ;

//    morphologyOperator = 2; //Gradient
//    morphologyKernelShape = 2; //Ellipse
//    morphologyKernelSize = 3; // 3x3

    morphologyOperator = 4; // Blackhat
    morphologyKernelShape = 2; //Ellipse
    morphologyKernelSize = 4; //4x4
    morphologyIternum = 1; //1 iter

    distTransformType = DIST_L2;
    distTransformMaskSize = 3;
}
