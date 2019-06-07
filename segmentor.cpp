#include "segmentor.h"
using namespace cv;
#include <QDebug>

Segmentor::Segmentor()
{
    numOfBlobs        = 0;
    minArea           = -1;
    maxArea           = -1;
    blobIDcolors            = std::vector<Vec3b>();
    blobAreas         = std::vector<int>();
    sortedBlobAreas   = std::vector<int>();
    selectedBlobAreas = std::vector<int>();
    selectedBlobIndices = std::vector<int>();
    blobPoints        = std::vector<std::vector<Point2i>>();
}

void Segmentor::loadDisplayRAWsImage(flirImg* fimg, int flags)
{
  using namespace std;
  this->fimg = fimg;
  qDebug()  << " Segmentor::loadDisplayRAWsImage - About to do it";
  src = imread(fimg->getDisplayRAWsImgpath(), flags);
  qDebug()  << " Segmentor::loadDisplayRAWsImage - Type of image read in : "
            << src.type();

  sParams = new SegmentorParams();
}

void Segmentor::changeBackgroundImage()
{
  // cvtColor(src, converted, COLOR_GRAY2BGR);
  // Change the background from white to black, since that will help later to extract
  // better results during the use of Distance Transform
  int count = 0;
  for( int x = 0; x < src.rows; x++ ) {
    for( int y = 0; y < src.cols; y++ ) {
      if ( src.at<Vec3b>(x, y) == Vec3b(255,255,255) ) {
        src.at<Vec3b>(x, y)[0] = 0;
        src.at<Vec3b>(x, y)[1] = 0;
        src.at<Vec3b>(x, y)[2] = 0;
        count++;
      }
    }
  }
  // std::cout<<"Type of bged image : "<<src.type()<<std::endl;
  // std::cout<<"Number of bg pixels : "<<count<<std::endl;
}

void Segmentor::filterImage()
{
  // Create a kernel that we will use for accuting/sharpening our image
  Mat filteringKernel = (Mat_<float>(3,3) <<
        1,  1, 1,
        1, -8, 1,
        1,  1, 1); // an approximation of second derivative, a quite strong kernel

  // do the laplacian filtering as it is
  // well, we need to convert everything in something more deeper then CV_8U
  // because the kernel has some negative values,
  // and we can expect in general to have a Laplacian image with negative values
  // BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
  // so the possible negative number will be truncated
  Mat imgLaplacian;
  Mat sharp = src; // copy source image to another temporary one
  filter2D(sharp, imgLaplacian, CV_32F, filteringKernel);
  src.convertTo(sharp, CV_32F);

  imgFiltered = sharp - imgLaplacian;

  // convert back to 8bits gray scale
  imgFiltered.convertTo(imgFiltered, CV_8UC3);
  // imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
  // imshow( "Laplace Filtered Image", imgLaplacian );
}

#include <QAbstractButton>

void Segmentor::morphologizeImage()
{
    int morphologyOperator, morphologyKernelShape, morphologyKernelSize, morphologyIternum;
    this->params()->getMorphologyParams(morphologyOperator, morphologyKernelShape, morphologyKernelSize, morphologyIternum);

    int operation = morphologyOperator + 2;
    Mat element = getStructuringElement( morphologyKernelShape,
                                         Size( 2*morphologyKernelSize + 1, 2*morphologyKernelSize+1 ),
                                         Point( morphologyKernelSize, morphologyKernelSize ));

    /// Apply the specified morphology operation
    morphologyEx( imgFiltered, imgMorphologized, operation, element, Point(-1,-1), morphologyIternum);
    //morphologyE

}

void Segmentor::binarizeImage()
{
  int binaryThreshLoVal, binaryThreshMaxVal, binaryThreshFlags;
  this->params()->getBinaryThreshParams(binaryThreshLoVal, binaryThreshMaxVal, binaryThreshFlags);
  //binaryMaxval   = 255;
  //thresholdingTypeFlags  = THRESH_BINARY | THRESH_OTSU;
  cvtColor(imgMorphologized, bw, COLOR_BGR2GRAY);
  threshold(bw, bw, binaryThreshLoVal, binaryThreshMaxVal, binaryThreshFlags);
  //imshow("binarized", bw);

}

void Segmentor::distanceTransformImage()
{
  int transformType, maskSize;
  this->params()->getDistTransformParams(transformType, maskSize);
  //maskSize = 5;
  distanceTransform(bw, distTransformed, transformType, maskSize);
  // Normalize the distance image for range = {0.0, 1.0}
  // so we can visualize and threshold it
  normalize(distTransformed, distTransformed, 0, 1., NORM_MINMAX);
  //imshow("distTransformed", distTransformed);

}

void Segmentor::obtainPeaksImage()
{
  // Threshold to obtain the peaks
  // This will be the markers for the foreground objects
  threshold(distTransformed, peaks, .1, 1., THRESH_BINARY);
  // Dilate a bit the dist image
  Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
  dilate(distTransformed, peaks, kernel1);
  //binarizeImage(peaks, peaks, 254, 255, THRESH_BINARY | THRESH_OTSU);
}


void Segmentor::watershedImage()
{
  using namespace std;
  markers = Mat::zeros(peaks.size(), CV_32SC1);
  // Create the CV_8U version of the distance image
  // It is needed for findContours()
  Mat peaks_8u;
  peaks.convertTo(peaks_8u, CV_8U);

  //vector<vector<Point> > contours;
  findContours(peaks_8u, contours, RETR_LIST, CHAIN_APPROX_NONE);
  contourSize = contours.size();

  for (size_t i = 0; i < contours.size(); i++){
    drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);
  }
  circle(markers, Point(5,5), 3, CV_RGB(255, 255, 255), -1);
  watershed(src, markers);
}

void Segmentor::generateRandomColors()
{
    blobIDcolors.clear();

    for (size_t i = 0; i < contourSize; i++){
      int b = theRNG().uniform(0, 200);
      int g = theRNG().uniform(0, 200);
      int r = theRNG().uniform(0, 200);
      Vec3b rc((uchar)b, (uchar)g, (uchar)r);
      blobIDcolors.push_back(rc);
    }
}

void Segmentor::initializeBlobAreasBlobPoints()
{
    for (size_t i = 0; i < contourSize; i++){
      blobAreas.push_back(0);
      Point2i nopoint(-1, -1);
      std::vector<Point2i> blobpointvec;
      blobpointvec.push_back(nopoint);
      blobPoints.push_back(blobpointvec);
    }
}

#include <map>
void Segmentor::fillRandomColors()
{
    int blobID;
    // Fill labeled objects with random colors
    for (int rowNum = 0; rowNum < markers.rows; rowNum++){
        for (int colNum = 0; colNum < markers.cols; colNum++){
            int index = markers.at<int>(rowNum,colNum);
            if (index > 0 && index <= static_cast<int>(contourSize)){
                blobID  = index - 1;
                segmentedColoredMat.at<Vec3b>(rowNum,colNum) = blobIDcolors[blobID];
                blobAreas.at(index-1)+= 1;
                Point2i p(rowNum, colNum);
                blobPoints.at(index-1).push_back(p);

                // blobArea[colors[index-1]]+=1;
            }
            else{
                segmentedColoredMat.at<Vec3b>(rowNum,colNum) = Vec3b(255,255,255);
            }
        }
    }
}




void Segmentor::colorizeImageSegments()
{
  using namespace std;
  segmentedColoredMat    = Mat::zeros(markers.size(), CV_8UC3);
  Mat mark = Mat::zeros(markers.size(), CV_8UC1);
  markers.convertTo(mark, CV_8UC1);
  bitwise_not(mark, mark);

  blobAreas.clear();
  blobPoints.clear();

  generateRandomColors();
  initializeBlobAreasBlobPoints();
  fillRandomColors();
  generateSortedBlobAreas();

  selected = segmentedColoredMat.clone();


  // for (size_t i = 0; i < contourSize; i++){
  //   cout << " Area of blob " << (i+1) << " : " << blobAreas.at(i) << endl;
  //   vector<Point2i> v = blobPoints.at(i);
  //   cout << " Num Points in blob " << (i+1) << " : " << v.size() << endl;
  // }

  qDebug() << " About to do something ";
  auto minElem = std::min_element(std::begin(blobAreas), std::end(blobAreas));
  auto maxElem = std::max_element(std::begin(blobAreas), std::end(blobAreas));
  minArea     = *minElem;
  maxArea     = *maxElem;
  numOfBlobs  = blobAreas.size();
  std::cout << " Segmentor : colorizeImageSegments() -- ";
  std::cout << " Segmentor Min Area : " << minArea;
  std::cout << " Segmentor Max Area : " << maxArea;
  std::cout << " Segmentor numColours : " << contourSize;
  std::cout << std::endl;
}



void Segmentor::generateSelectedBlobIndices()
{
    // qDebug() << " Segmentor : updateWithSelectedBlobAreas() -- begun";
    selectedBlobIndices.clear();

    std::vector<int>::iterator it;
    // auto it = blobAreas.begin();

    for(int sArea : selectedBlobAreas){
        it = std::find(blobAreas.begin(), blobAreas.end(), sArea);
        if(it != blobAreas.end()){
            // qDebug() << " Segmentor : generateSelectedBlobIndices : (Area, Index) : "
            //          << sArea << " - " << it - blobAreas.begin();
            selectedBlobIndices.push_back(it - blobAreas.begin());
        }
    }
}

void Segmentor::fillSelectedAreasRandomColors()
{
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++){
        for (int j = 0; j < markers.cols; j++){
            int index = markers.at<int>(i,j);
            if(std::find(selectedBlobIndices.begin(), selectedBlobIndices.end(), (index-1)) != selectedBlobIndices.end()){
                selected.at<Vec3b>(i,j) = blobIDcolors[index-1];
                // blobArea[colors[index-1]]+=1;
            }
            else{
                selected.at<Vec3b>(i,j) = Vec3b(255,255,255);
            }
        }
    }
}







std::vector<Vec3b>                Segmentor::getColors()     { return blobIDcolors;     }
std::vector<int>                  Segmentor::getBlobAreas()  { return blobAreas;  }
std::vector<int>            Segmentor::getSortedBlobAreas()  { return sortedBlobAreas; }
std::vector<int>            Segmentor::getSelectedBlobAreas(){ return selectedBlobAreas; }
std::vector<std::vector<Point2i>> Segmentor::getBlobPoints() { return blobPoints; }
int                               Segmentor::getMinArea()    { return minArea;    }
int                               Segmentor::getMaxArea()    { return maxArea;    }


int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

void Segmentor::generateSortedBlobAreas()
{
    // qDebug() << "Segmentor : generateSortedBlobAreas() -- begun";
    sortedBlobAreas.clear();

    std::vector<int>areasV = getBlobAreas();
    int areaSize = areasV.size();
    int areasA[areaSize];
    std::copy(areasV.begin(), areasV.end(), areasA);

    int numOfBlobs = getBlobAreas().size();
    std::qsort(areasA, numOfBlobs, sizeof(int), compare);

    for(int x : areasA){
        sortedBlobAreas.push_back(x);
    }
    // qDebug() << "Segmentor : generateSortedBlobAreas() -- fini";
}

void Segmentor::generateSelectedBlobAreas(int min, int max)
{
    // qDebug() << "Segmentor : generateSelectedBlobAreas() with "
    //         << min << " and " << max;
    selectedBlobAreas.clear();
    std::vector<int> areasS = getSortedBlobAreas();
    int areasSA[areasS.size()];

    std::copy(areasS.begin(), areasS.end(), areasSA);

    for(int index=min; index <= max; index++){
        selectedBlobAreas.push_back(areasSA[index]);
        // qDebug() << " .... Selected Area : " << areasSA[index];
    }

    // qDebug() << "Segmentor : generateSelectedBlobAreas() done";
}

int Segmentor::getIsPointKaBlobID(QPoint p)
{


    int index  = markers.at<int>(p.y(), p.x());
    int blobID = index - 1;
    qDebug() << QString(" Segmentor::getIsPointKaBlob(%1, %2) - %3").arg(p.x()).arg(p.y()).arg(blobID);
    return blobID;
}


int Segmentor::getIsPointKaBlobArea(QPoint p)
{
    int blobID = getIsPointKaBlobID(p);
    int blobArea;
    try{
        blobArea = blobPoints.at(blobID).size();
    }
    catch(std::exception e){
        qDebug() << e.what();
    }
    return blobArea;
}

std::vector<Point2i> Segmentor::getIsPointKeBlobKePoints(QPoint p)
{
    int blobID = getIsPointKaBlobID(p);
    std::vector<Point2i> pointsOfASingleBlob;
    try{
        pointsOfASingleBlob = blobPoints.at(blobID);
        qDebug() << "Size of points returned " << pointsOfASingleBlob.size();
    }
    catch(std::exception e){
        qDebug() << e.what();
    }
    return pointsOfASingleBlob;
}

double Segmentor::getIsPointKeBlobKaAvgTemp(QPoint p)
{
    return fimg->calcBlobAvgTemp(getIsPointKeBlobKePoints(p));

    //    // qDebug() << " tgfsTabbedWindow::isPointKeBlobKaTempNikaaloMadarchod(QPointF p) - Inside madarchod blob nikaalo " << p.x() << ", " << p.y();
    //    std::vector<Point2i> pts;
    //    pts = segmentor->getIsPointKaBlob(p);
    //    std::cout << " Num of points in this blob " << pts.size() << std::endl;
    //    double avgBlobTemp;
    //    avgBlobTemp = fimg->calcBlobAvgTemp(pts);
    //    emit blobAvgTempLeleBhosdike(avgBlobTemp);
    //    // std::cout << "Avg blob Temp " << avgBlobTemp << endl;
    //    // std::cout << "Blob num " << blobNum << std::endl;
}


QString Segmentor::getStatusString(QPoint imgPos)
{
    int colNum = imgPos.x();
    int rowNum = imgPos.y();

    int    blobID      = getIsPointKaBlobID(imgPos);
    int    blobArea    = getIsPointKaBlobArea(imgPos);
    double blobAvgTemp = getIsPointKeBlobKaAvgTemp(imgPos);

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
                           + QString::number(blobArea)
                           + " : Blob temp "
                           + QString::number(blobAvgTemp);
    return statusString;
}


void Segmentor::segmentImage()
{
  changeBackgroundImage();   emit signalSegmentorProgress(10);
  // qDebug() << " Segmentor : segmentImage() - Background done ";
  filterImage();             emit signalSegmentorProgress(20);
  morphologizeImage();       emit signalSegmentorProgress(30);
  binarizeImage();           emit signalSegmentorProgress(40);
  distanceTransformImage();  emit signalSegmentorProgress(50);
  obtainPeaksImage();        emit signalSegmentorProgress(60);
  watershedImage();          emit signalSegmentorProgress(70);
  // qDebug() << " Segmentor : segmentImage() - Watershedding done";
  colorizeImageSegments();   emit signalSegmentorProgress(100);
  updateWithSelectedBlobAreas(0, numOfBlobs - 1);
  // qDebug() << " Segmentor : segmentImage() - Colorization done";

  // calculateBlobAreas();
}

void Segmentor::updateWithSelectedBlobAreas(int min, int max)
{

    generateSortedBlobAreas();
    generateSelectedBlobAreas(min, max);
    generateSelectedBlobIndices();
    fillSelectedAreasRandomColors();

    // qDebug() << " Segmentor : updateWithSelectedBlobAreas() -- fini";
}
