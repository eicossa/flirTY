#include "flirimg.h"
#include <fstream>
#include <QDebug>

void flirImg::writeRaw2TempValues()
{
    std::ofstream myfile;
    myfile.open(this->unorderedRAWsCSVFile.toStdString());
    qDebug() << " flirImg::writeRaw2TempValues() - Opening file "
             << QString::fromStdString(this->unorderedRAWsCSVFile.toStdString());

    int rawthermalimgwidth  = getRAWThermalImageWidth();
    int rawthermalimgheight = getRAWThermalImageHeight();

    if(myfile.is_open()){
        qDebug() << "flirImg::writeRaw2TempValue ... yet another line";
        for(int rowNum=0; rowNum < rawthermalimgheight; rowNum++){
          for(int colNum=0; colNum < rawthermalimgwidth; colNum++){
            myfile << getStatusString(rowNum, colNum).toStdString() << endl;
          }
        }
    }
    else qDebug() << " flirImg::writeRaw2TempValues() -- Unable to open file ";
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
       << " | Tmin    : "        << setw(10) << tmin << endl;
  cout << " | PlanckR2       : " << setw(10) << planckr2
       << " | RAWmin         : " << setw(10) << rawMin
       << " | Tmax    : "        << setw(10) << tmax << endl;
  cout << " | PlanckB        : " << setw(10) << planckb
       << " | RAWrefl        : " << setw(10) << rawRefl << endl;
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
    std::ifstream myfile(this->metadataFile.toStdString());
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
    else cerr << "Unable to open file " << this->metadataFile.toStdString() << endl;
}

void flirImg::writePrereadMetadata()
{
    std::ofstream myfile;
    myfile.open(this->metadataFile.toStdString());
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
                 << QString::fromStdString(this->metadataFile.toStdString());
    }
}


#include <sys/stat.h>
bool flirImg::doesFileExist (const QString& fileName) {
  struct stat buffer;
  return (stat (fileName.toStdString().c_str(), &buffer) == 0);
}

bool flirImg::readFileIntoMat(QString aPath, Mat &aMat, cv::ImreadModes flags)
{
    aMat = imread(aPath.toStdString(), flags);

    if(!aMat.empty()) {
      return true;
    } else {
        qDebug() << "flirImg::checkIfImgFileExists:: "
                 << QString::fromStdString(emsgs->getThermalImgErrorMsg())
                 << " | "
                 << aPath;
        return false;
    }
}

bool flirImg::writeMatToFile(QString fPath, cv::Mat aMat)
{
    imwrite(fPath.toStdString(), aMat);
}
//void flirImg::writeOrderedRAWsMatToFile()
//{
//
//}
