#include "flirimg.h"
#include "cmdstrings.h"
using namespace cv;

#include <sys/stat.h>
inline bool doesFileExist (const std::string& fileName) {
  struct stat buffer;
  return (stat (fileName.c_str(), &buffer) == 0);
}


flirImg::flirImg()
{
  printCounter = 0;
  fmd     = NULL;
  imgpath = "";
  reorderedRAWs_imgpath ="";
  emsgs   = new ErrMsgs();
  temperatures = NULL;

  fmd          = new flirImgMetadata();
}

void flirImg::initializePaths(std::string fimgpath)
{
    imgpath                        = fimgpath;
    unorderedRAWs_imgpath          = imgpath + std::string(".raws");
    displayRAWs_imgpath            = imgpath + std::string(".raws.png");
    displayRGB_imgpath             = imgpath + std::string(".rgb.png");
    repaletted_imgpath             = imgpath + std::string(".repaletted.png");

    tempsFile               = imgpath + std::string(".raw2temps_rwaala.csv");
    unorderedRAWsFile       = imgpath + std::string(".unorderedRAWs.csv");
    orderedRAWsFile         = imgpath + std::string(".orderedRAWs.csv");

    metadataFile            = imgpath + std::string(".metadata.txt");
}

void flirImg::readAllTags()
{

    QStringList allFlirTags;
    qDebug() << "flirImgMetadata::readAllBasicTags::" << endl;
    allFlirTags = shX->extractAllTagsInOneGo();

    qDebug() << "flirImgMetadata::readAllMetadataTags() : read in all tags";
    //QStringList allFlirTagsLines = allFlirTags.split('\n');
    for ( const auto& kvpairStr : allFlirTags  )
    {
        QStringList kvpair = kvpairStr.split(":");
        if(kvpair.size() == 2){
          // qDebug() << kvpairStr << "\n-";
          QString key                = kvpair.at(0).simplified();
          QString val                = kvpair.at(1).simplified();
          //qDebug() << "key " << key;
          //qDebug() << "val " << val;
          flirTagsMap.insert(key, val);
        }
        //QStringList kvpair = kvpairStr.split(":");
    }
    imagewidth = flirTagsMap["Exif Image Width"].toInt();
    imageheight = flirTagsMap["Exif Image Height"].toInt();

    // qDebug() << " PlanckR1 " << flirTagsMap["Planck R1"];
    planckr1 = flirTagsMap["Planck R1"].toDouble();
    planckr2 = flirTagsMap["Planck R2"].toDouble();
    planckb  = flirTagsMap["Planck B"].toDouble();
    planckf  = flirTagsMap["Planck F"].toDouble();
    plancko  = flirTagsMap["Planck O"].toDouble();


    emis     = flirTagsMap["Emissivity"].toDouble();

    rawvaluerange  = flirTagsMap["Raw Value Range"].toDouble();
    rawvaluemedian = flirTagsMap["Raw Value Median"].toDouble();

    ata1 = flirTagsMap["Atmospheric Trans Alpha 1"].toDouble();
    ata2 = flirTagsMap["Atmospheric Trans Alpha 2"].toDouble();
    atb1 = flirTagsMap["Atmospheric Trans Beta 1"].toDouble();
    atb2 = flirTagsMap["Atmospheric Trans Beta 2"].toDouble();
    atx  = flirTagsMap["Atmospheric Trans X"].toDouble();

    rawthermalimagewidth  = flirTagsMap["Raw Thermal Image Width"].toInt();
    rawthermalimageheight = flirTagsMap["Raw Thermal Image Height"].toInt();


    cmodel = flirTagsMap["Camera Model"].toStdString();


    RawThermalImageType = flirTagsMap["Raw Thermal Image Type"].toStdString();
    palName             = flirTagsMap["Palette Name"].toStdString();

    focusDist           = flirTagsMap["Focus Distance"].split(" ").at(0).toDouble();
    objDist             = flirTagsMap["Object Distance"].split(" ").at(0).toDouble();
    //qDebug() << "focusDist " << flirTags["Focus Distance"] << " " << focusDist
    //         << " objDist " << flirTags["Object Distance"] << " " << objDist;

    tref     = flirTagsMap["Reflected Apparent Temperature"].split(" ").at(0).toDouble();
    atmTemp  = flirTagsMap["Atmospheric Temperature"].split(" ").at(0).toDouble();
    qDebug() << " ATMTEMP " << atmTemp;
    IRWTemp  = flirTagsMap["IR Window Temperature"].split(" ").at(0).toDouble();
    qDebug() << " IRWTEMP " << IRWTemp;
    IRWTrans = flirTagsMap["IR Window Transmission"].toDouble();
    relHum   = flirTagsMap["Relative Humidity"].split(" ").at(0).toDouble();
    qDebug() << " RELHUM " << relHum;

}


void flirImg::processImage(std::string fimgpath)
{
  initializePaths(fimgpath);
  shX = new shellExecutor(imgpath, unorderedRAWs_imgpath, displayRAWs_imgpath);
  readAllTags();

  shX->readyForExtraction(rawthermalimagewidth, rawthermalimageheight);
  shX->executeRAWsExtractionCmd();

  if(didUnorderedRAWsExtractionSucceed()){
      createOrderedRAWsMat();
      calcTempForEveryPixel();


      shX->executeConvertOrderedRAWsToPNG();

      if(didDisplayRAWsConversionSucceed()){
          //shX->executeRepaletteImage();
      }
      else{
          qDebug() << " flirImg::processImage -- For some reason, couldn't convert RAWs to a viewable PNG";
          exit;
      }
      writeRaw2TempValues();
  }
  else{
      qDebug() << "flirImg::processImage -- RAWs extraction failed";
      exit;
  }

  //shX->executeExtractEmbeddedRGBImage();

  calcTminTmax();
  printImageSummary();
}

#include <inttypes.h>
#include <arpa/inet.h>
void flirImg::createOrderedRAWsMat()
{
    //int preOrderedRAWval, reOrderedRAWval;
    qDebug() << "Size of int " << sizeof(int) << "size of unsigned short " << sizeof(unsigned short);
    unsigned short x, y, x1, x2, x3, preOrderedRAWval, reOrderedRAWval;
    int width = this->unorderedRAWsMat.size().width;
    int height = this->unorderedRAWsMat.size().height;
    qDebug() << "flirImg::createReorderedRAWsMat of size " << width << " x " << height;
    orderedRAWsMat = Mat::zeros(height, width, CV_16U);

    for(int i=0; i < height; i++){
      //cout << " i : " << i << endl;
      for(int j=0; j < width; j++){
        //cout << " (i, j) : " << i << ", " << j << " | ";
        preOrderedRAWval = unorderedRAWsMat.at<unsigned short>(i, j);
        x = preOrderedRAWval;
        x1 = x >> 8;
        x2 = (x & 0x0ff);
        x3 = x2 << 8;
        y = x1 + x3;
        reOrderedRAWval = y;
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


#include <QDebug>
bool flirImg::didUnorderedRAWsExtractionSucceed()
{
    unorderedRAWsMat = imread(unorderedRAWs_imgpath, cv::IMREAD_UNCHANGED);

    if(!unorderedRAWsMat.empty()) {
      return true;
    } else {
        qDebug() << "flirImg::loadRAWThermalImage:: "
                 << QString::fromStdString(emsgs->getThermalImgErrorMsg())
                 << " | "
                 << QString::fromStdString(unorderedRAWs_imgpath);
        return false;
    }
}


bool flirImg::didDisplayRAWsConversionSucceed()
{
    displayRAWsMat = imread(displayRAWs_imgpath, cv::IMREAD_COLOR);


    if(!displayRAWsMat.empty()) {
      return true;
    } else {
        qDebug() << "flirImg::loadRAWThermalImage:: "
                 << QString::fromStdString(emsgs->getThermalImgErrorMsg())
                 << " | "
                 << QString::fromStdString(displayRAWs_imgpath);
        return false;
    }
}

double flirImg::calcBlobAvgTemp(std::vector<Point2i> blobPts)
{
    double avgBlobTemp    = 0.0;
    double sumOfBlobTemps = 0.0;
    for(int i = 0; i < blobPts.size(); i++){
        Point2i pt = blobPts.at(i);
        int y = pt.y;
        int x = pt.x;
        if(x!=-1 && y!=-1)
          sumOfBlobTemps += temperatures.at<double>(x, y);
    }
    int numOfPoints = blobPts.size();
    avgBlobTemp = sumOfBlobTemps/numOfPoints;
    return avgBlobTemp;
}

QString flirImg::getStatusString(int rowNum, int colNum)
{
    QString rowString           = QString::number(rowNum).leftJustified(4);
    QString colString           = QString::number(colNum).leftJustified(4);
    QString orderedRAWvalString = QString::number(orderedRAWsMat.at<unsigned short>(rowNum, rowNum));
    QString tempString          = QString::number(temperatures.at<double>(rowNum, rowNum));

    QString statusString = rowString
                           + ", "
                           + colString
                           + " : "
                           + orderedRAWvalString
                           + " - "
                           + tempString;

    return statusString;
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



double flirImg::getPixelTemperature(int rowNum, int colNum)
{
    return temperatures.at<double>(rowNum, colNum);
}



int flirImg::getOrderedRAWval(int rowNum, int colNum)
{
    return orderedRAWsMat.at<int>(rowNum, colNum);
}

void flirImg::writeRaw2TempValues()
{
    std::ofstream myfile;
    myfile.open(this->unorderedRAWsFile);
    qDebug() << " flirImg::writeRaw2TempValues() - Opening file "
             << QString::fromStdString(this->unorderedRAWsFile);

    int rawthermalimgwidth  = getRAWThermalImageWidth();
    int rawthermalimgheight = getRAWThermalImageHeight();

    if(myfile.is_open()){
        for(int rowNum=0; rowNum < rawthermalimgheight; rowNum++){
          for(int colNum=0; colNum < rawthermalimgwidth; colNum++){
            myfile << getStatusString(rowNum, colNum).toStdString() << endl;
          }
        }
    }
    else qDebug() << " flirImg::writeRaw2TempValues() -- Unable to open file ";
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
    double tempC   = calcTempFromRawObjValue(rawobjvalue);
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
    double raw_refl1_attn = (1-E)/E*raw_refl1; // what the fuck, why didn't they use parens here ?
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
    if(raw == 14128 && printCounter==0){
        printCounter+=1;
        qDebug() << " RTemp " << tref << " ATemp " << ATemp << " IRWTemp " << IRWTemp;
        qDebug() << " For raw " << raw << " raw_obj = " << raw_obj;

        qDebug() << " raw_refl1 " << raw_refl1 << " raw_refl1_attn " << raw_refl1_attn;
        qDebug() << " raw_atm1  "  << raw_atm1  << " raw_atm1_attn "  << raw_atm1_attn;
        qDebug() << " raw_wind  "  << raw_wind  << " raw_wind_attn "  << raw_wind_attn;
        qDebug() << " raw_refl2 " << raw_refl2 << " raw_refl2_attn " << raw_refl2_attn;
        qDebug() << " raw_atm2  "  << raw_atm2  << " raw_atm2_attn "  << raw_atm2_attn;

    }

    return raw_obj;
}





double flirImg::calcTempFromRawObjValue(double rawobjvalue)
{
    double tempC = planckb/log(planckr1/(planckr2*(rawobjvalue + plancko))+planckf)-273.15;
    return tempC;
}



void flirImg::printCalcedMetadata()
{
  cout << "\n\n\nRaws calculated" << endl;
  cout << "RAWmax         : "     << rawMax         << endl;
  cout << "RAWmin         : "     << rawMin         << endl;
  cout << "RAWrefl        : "     << rawRefl        << endl;
  cout << "RAWmaxobj      : "     << rawMaxObj      << endl;
  cout << "RAWminobj      : "     << rawMinObj      << endl;

  cout << "\n\n Tmin, Tmax ..." << endl;
  cout << "Tmin    : "          << tmin   << endl;
  cout << "Tmax    : "          << tmax   << endl;

  cout << "\n\n Smax, Smin, Sdelta calculated ..." << endl;
  cout << "Smax    : "          << smax   << endl;
  cout << "Smin    : "          << smin   << endl;
  cout << "Sdelta  : "          << sdelta << endl;

}

void flirImg::printReadMetadata()
{
  cout << "PlanckR1       : " << planckr1       << endl;
  cout << "PlanckR2       : " << planckr2       << endl;
  cout << "PlanckB        : " << planckb        << endl;
  cout << "PlanckF        : " << planckf        << endl;
  cout << "PlanckO        : " << plancko        << endl;
  cout << "Tref           : " << tref           << endl;
  cout << "Emis           : " << emis           << endl;
  cout << "RawValueMedian : " << rawvaluemedian << endl;
  cout << "RawValueRange  : " << rawvaluerange  << endl;

  cout << "--------------------------------" << endl;

  cout << "ImageWidth  : " << imagewidth << endl;
  cout << "ImageHeight : " << imageheight << endl;

  cout << "RawThermalImageWidth  : " << rawthermalimagewidth << endl;
  cout << "RawThermalImageHeight : " << rawthermalimageheight << endl;

  cout << "--------------------------------" << endl;
}


void flirImg::printImageSummary()
{
  //cout << "\n\n\nExiftags read .... "            << endl;
  cout << " | PlanckR1       : " << setw(10) << planckr1
       << " | RAWmax         : " << setw(10) << rawMax
       << " | Tmin    : "        << setw(10) << tmin
       << " | Smax    : "        << setw(10) << smax   << endl;
  cout << " | PlanckR2       : " << setw(10) << planckr2
       << " | RAWmin         : " << setw(10) << rawMin
       << " | Tmax    : "        << setw(10) << tmax
       << " | Smin    : "        << setw(10) << smin   << endl;
  cout << " | PlanckB        : " << setw(10) << planckb
       << " | RAWrefl        : " << setw(10) << rawRefl
       << setw(23) << ""
       << " | Sdelta   : " << setw(10) << sdelta << endl;
  cout << " | PlanckF        : " << setw(10) << planckf
       << " | RAWmaxobj      : " << setw(10) << rawMaxObj << endl;
  cout << " | PlanckO        : " << setw(10) << plancko
       << " | RAWminobj      : " << setw(10) << rawMinObj << endl;
  cout << " | Tref           : " << setw(10) << tref           << endl;
  cout << " | Emis           : " << setw(10) << emis           << endl;
  cout << " | RawValueMedian : " << setw(10) << rawvaluemedian << endl;
  cout << " | RawValueRange  : " << setw(10) << rawvaluerange  << endl;
}

void flirImg::loadPrereadMetadata()
{
    std::ifstream myfile(this->metadataFile);
    if (myfile.is_open()) {

        std::string line;
        getline(myfile, line); planckr1 = stod(line);
        getline(myfile, line); planckr2 = stod(line);
        getline(myfile, line); planckb  = stod(line);
        getline(myfile, line); planckf  = stod(line);
        getline(myfile, line); plancko  = stod(line);

        getline(myfile, line); tref     = stod(line);
        getline(myfile, line); emis     = stod(line);

        getline(myfile, line); rawvaluemedian = stod(line);
        getline(myfile, line); rawvaluerange  = stod(line);

        getline(myfile, line); imagewidth            = stod(line);
        getline(myfile, line); imageheight           = stod(line);
        getline(myfile, line); rawthermalimagewidth  = stod(line);
        getline(myfile, line); rawthermalimageheight = stod(line);


        myfile.close();
    }
    else cerr << "Unable to open file " << this->metadataFile << endl;
}

void flirImg::writePrereadMetadata()
{
    std::ofstream myfile;
    myfile.open(this->metadataFile);
    qDebug() << " flirImgMetadata::writePrereadMetadata() - WRITING THE METADATA ";

    if(myfile.is_open()){
        myfile << planckr1 << endl;
        myfile << planckr2 << endl;
        myfile << planckb << endl;
        myfile << planckf << endl;
        myfile << plancko << endl;
        myfile << tref << endl;
        myfile << emis << endl;
        myfile << rawvaluemedian << endl;
        myfile << rawvaluerange << endl;
        myfile << imagewidth << endl;
        myfile << imageheight << endl;
        myfile << rawthermalimagewidth << endl;
        myfile << rawthermalimageheight << endl;
        myfile.close();
    }
    else {
        qDebug() << " flirImgMetadata::writePrereadMetadata() - Unable to open file "
                 << QString::fromStdString(this->metadataFile);
    }
}





