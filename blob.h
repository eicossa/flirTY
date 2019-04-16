#ifndef BLOB_H
#define BLOB_H
#include <opencv2/core/core.hpp>
using namespace cv;

class Blob{
    int                  blobId;
    int                  markerIndex;
    Vec3b                color;
    int                  area;
    std::vector<Point2i> pts;
    double               avgTemp;
public:
    Blob();
    void makeNewBlob(int blobId, int markerIndex);
    void incrementArea();
    void isPointInThisBlob();
    void calcAvgTempOfBlob();

};

#endif // BLOB_H
