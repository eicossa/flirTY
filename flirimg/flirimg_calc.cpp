#include "flirimg.h"
#include <QDebug>


double flirImg::calcBlobAvgTemp(std::vector<Point2i> blobPts)
{
    //qDebug() << "flirImg::calcBlobAvgTemp with " << blobPts.size();
    double avgBlobTemp    = 0.0;
    double sumOfBlobTemps = 0.0;
    for(int i = 0; i < blobPts.size(); i++){
        Point2i pt = blobPts.at(i);
        int col = pt.y;
        int row = pt.x;
        if(row!=-1 && col!=-1){
            sumOfBlobTemps += getPixelTemperature(row, col);
        }
    }
    double numOfPoints = (double)blobPts.size();
    avgBlobTemp = sumOfBlobTemps/numOfPoints;
    return avgBlobTemp;
}


void flirImg::calcTminTmax()
{
    rawMax = rawvaluemedian + (rawvaluerange/2);
    rawMin = rawMax - rawvaluerange;

    tmin = calcTempForOnePixel(rawMin);
    tmax = calcTempForOnePixel(rawMax);
    qDebug() << "Tmin : " << tmin << " | " << "Tmax : " << tmax;
}

double flirImg::calcTempForOnePixel(double thermalintensityvalue)
{
    double rawobjvalue = calcRawObjForRawThermal(thermalintensityvalue);
    double tempC       = calcTempFromRawObjValue(rawobjvalue);
    return tempC;
}

#include <cmath>

double flirImg::calcRawObjForRawThermal(double thermalintensityvalue)
{
    // double E        = 1;
    // double OD       = 1;
    // double RTemp    = 20;
    // double ATemp    = RTemp;
    // double IRWTemp  = RTemp;
    // double IRT      = 1;
    // double RH       = 50;
    // double planckr1 = 21106.77;
    // double planckb  = 1501;
    // double planckf  = 1;
    // double plancko  = -7340;
    // double planckr2 = 0.012545258;
    // double ATA1     = 0.006569;
    // double ATA2     = 0.01262;
    // double ATB1     = -0.002276;
    // double ATB2     = -0.00667;
    // double ATX      = 1.9;
    // emiss.wind      = 1 - IRT;
    // refl.wind       = 0 # anti-reflective coating on window

    int    raw     = thermalintensityvalue;
    double E       = emis;
    double OD        = objDist;
    double RTemp   = tref;
    double ATemp     = atmTemp;
    double IRWTemp   = this->IRWTemp;
    double IRT       = this->IRWTrans;
    double RH        = relHum;
    double planckr1 = this->planckr1;
    double planckb  = this->planckb;
    double planckf  = this->planckf;
    double plancko  = this->plancko;
    double planckr2 = this->planckr2;
    double ATA1     = this->ata1;
    double ATA2     = this->ata2;
    double ATB1     = this->atb1;
    double ATB2     = this->atb2;
    double ATX      = this->atx;
    double emiss_wind = 1 - IRT;
    double refl_wind  = 0;


    double h2o = (RH/100)*exp(1.5587
                              + 0.06939 * ATemp
                              - 0.00027816 * pow(ATemp, 2)
                              + 0.00000068455 * pow(ATemp, 3));
    double tau1 = ATX * exp(-sqrt(OD/2) * (ATA1 + ATB1 * sqrt(h2o)))
                  + (1 - ATX) * exp(-sqrt(OD/2) * (ATA2 + ATB2 * sqrt(h2o)));
    double tau2 = ATX * exp(-sqrt(OD/2) * (ATA1 + ATB1 * sqrt(h2o)))
                  + (1-ATX) * exp(-sqrt(OD/2) * (ATA2 + ATB2 * sqrt(h2o)));
    double raw_refl1 = planckr1/(planckr2*(exp(planckb/(RTemp+273.15))-planckf)) - plancko;
    double raw_refl1_attn = (1-E)/E*raw_refl1;
    double raw_atm1 = planckr1/(planckr2*(exp(planckb/(ATemp+273.15))-planckf)) - plancko;
    double raw_atm1_attn = (1-tau1)/E/tau1 * raw_atm1;
    double raw_wind = planckr1/(planckr2*(exp(planckb/(IRWTemp+273.15))-planckf)) - plancko;
    double raw_wind_attn = emiss_wind/E/tau1/IRT * raw_wind;
    double raw_refl2 = planckr1/(planckr2*(exp(planckb/(RTemp+273.15))-planckf)) - plancko;
    double raw_refl2_attn = refl_wind/E/tau1/IRT * raw_refl2;
    double raw_atm2 = planckr1/(planckr2*(exp(planckb/(ATemp+273.15))-planckf)) - plancko;
    double raw_atm2_attn = (1-tau2)/E/tau1/IRT/tau2 * raw_atm2;
    double raw_obj = (raw/E/tau1/IRT/tau2
                      - raw_atm1_attn
                      - raw_atm2_attn
                      - raw_wind_attn
                      - raw_refl1_attn
                      - raw_refl2_attn);
//    if(raw == 14128 && printCounter==0){
//        printCounter+=1;
//        qDebug() << " RTemp " << tref << " ATemp " << ATemp << " IRWTemp " << IRWTemp;
//        qDebug() << " For raw " << raw << " raw_obj = " << raw_obj;

//        qDebug() << " raw_refl1 " << raw_refl1 << " raw_refl1_attn " << raw_refl1_attn;
//        qDebug() << " raw_atm1  "  << raw_atm1  << " raw_atm1_attn "  << raw_atm1_attn;
//        qDebug() << " raw_wind  "  << raw_wind  << " raw_wind_attn "  << raw_wind_attn;
//        qDebug() << " raw_refl2 " << raw_refl2 << " raw_refl2_attn " << raw_refl2_attn;
//        qDebug() << " raw_atm2  "  << raw_atm2  << " raw_atm2_attn "  << raw_atm2_attn;

//    }

    return raw_obj;
}





double flirImg::calcTempFromRawObjValue(double rawobjvalue)
{
    double tempC = planckb/log(planckr1/(planckr2*(rawobjvalue + plancko))+planckf)-273.15;
    return tempC;
}



void flirImg::calcTempForEveryPixel()
{

  temperatures = Mat::zeros(this->unorderedRAWsMat.size(), CV_64F);

  for(int colNum=0; colNum<rawthermalimagewidth; colNum++){
    for(int rowNum=0; rowNum<rawthermalimageheight; rowNum++){
      temperatures.at<double>(rowNum, colNum) = calcTempForOnePixel(orderedRAWsMat.at<unsigned short>(rowNum, colNum));
    }
    double progressLevel = ((double)((double)colNum/(double)rawthermalimagewidth)) * 100;
    emit signalFlirImgProgress((int)progressLevel);
  }
}

unsigned short flirImg::changeByteOrder(unsigned short wrongByteOrder)
{
    unsigned short x, y, x1, x2, x3;
    unsigned short reOrderedRAWval;
    x               = wrongByteOrder;
    x1              = x >> 8;
    x2              = (x & 0x0ff);
    x3              = x2 << 8;
    y               = x1 + x3;
    reOrderedRAWval = y;
    return reOrderedRAWval;
}

#include <inttypes.h>
//#include <arpa/inet.h>




void flirImg::changeByteOrderOfRAWsMat()
{
    //int preOrderedRAWval, reOrderedRAWval;
    qDebug() << "flirImg::generateOrderedRAWsMat "
             << " Size of int " << sizeof(int)
             << " Size of unsigned short " << sizeof(unsigned short);
    unsigned short x, y, x1, x2, x3, unOrderedRAWval, reOrderedRAWval;
    int width  = unorderedRAWsMat.size().width;
    int height = unorderedRAWsMat.size().height;
    qDebug() << "flirImg::generateOrderedRAWsMat of size " << width << " x " << height;
    orderedRAWsMat = Mat::zeros(height, width, CV_16U);

    for(int i=0; i < height; i++){
      for(int j=0; j < width; j++){
          unOrderedRAWval = unorderedRAWsMat.at<unsigned short>(i, j);
          reOrderedRAWval = changeByteOrder(unOrderedRAWval);

          if(i==0 && j==0)
            qDebug() << " x, x1, x3, y " << x << x1 << x3 << y;
          orderedRAWsMat.at<unsigned short>(i, j) = reOrderedRAWval;
          //don't use int here ... it crashes at runtime
       }
    }
    qDebug() << "Done with trying to reorder every RAW value";
    for(int i=320; i<330; i++)
        qDebug() << unorderedRAWsMat.at<unsigned short>(320, i)
                 << " translates to "
                 << orderedRAWsMat.at<unsigned short>(320, i);
}
