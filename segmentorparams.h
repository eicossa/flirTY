#ifndef SEGMENTORPARAMS_H
#define SEGMENTORPARAMS_H

class SegmentorParams
{
public:
    SegmentorParams();

    void setMorphologyOperator(int op) { morphologyOperator = op; }
    void setMorphologKernelShape(int shp) { morphologyKernelShape = shp; }
    void setMorphologyKernelSize(int size) { morphologyKernelSize = size; }
    void setMorphologyIternum(int num) { morphologyIternum = num; }
    void getMorphologyParams(int &op, int &shp, int &size, int &num)
    { op = morphologyOperator; shp = morphologyKernelShape; size = morphologyKernelSize;
      num = morphologyIternum; }

    void setBinaryThresholdLoVal(int lo) { binaryThreshLoVal = lo; }
    void setBinaryThreshMaxVal(int max) { binaryThreshMaxVal = max; }
    void setBinaryThreshFlags(int flags) { binaryThreshFlags = flags; }
    void getBinaryThreshParams(int &lo, int &max, int &flags)
    { lo = binaryThreshLoVal; max = binaryThreshMaxVal; flags = binaryThreshFlags; }

    void setDistTransformType(int type) { distTransformType = type; }
    void setDistTransformMaskSize(int size) { distTransformMaskSize = size; }
    void getDistTransformParams(int &type, int &size)
    { type = distTransformType; size = distTransformMaskSize; }
private:
    int morphologyOperator, morphologyKernelShape, morphologyKernelSize, morphologyIternum;
    int binaryThreshLoVal, binaryThreshMaxVal, binaryThreshFlags;
    int distTransformType, distTransformMaskSize;
};

#endif // SEGMENTORPARAMS_H
