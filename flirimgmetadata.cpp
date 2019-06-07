#include "flirimgmetadata.h"
<<<<<<< HEAD

#include <QDebug>
#include <QtGlobal>
#include "exec.h"


=======
#include <sys/stat.h>

inline bool exists_test3 (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3


flirImgMetadata::flirImgMetadata()
{
  imgpath = "";
<<<<<<< HEAD
=======
  planckr1 = 16627.6;
  planckr2 = 0.0145961;
  planckb = 1429.3;
  planckf = 1;
  plancko = -4420;

  tref = 25;
  emis = 0.95;

  rawvaluerange  = 679;
  rawvaluemedian = 14611;

  // read basic metadata
  imagewidth            = 457;
  imageheight           = 343;
  rawthermalimagewidth  = 640;
  rawthermalimageheight = 480;

  // calculated values
  rawmin = 14271.5;
  rawmax = 14950.5;
  rawrefl = 13931.3;
  rawminobj = 14289.4;
  rawmaxobj = 15004.1;

  tmin = 27.2968;
  tmax = 31.7379;

  smin = 300.333;
  smax = 304.561;
  sdelta = 4.22767;

  metadataFile = imgpath + std::string(".metadata.txt");
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
}

void flirImgMetadata::loadMetadata(std::string imgpath, std::string thermimgpath)
{
    this->imgpath       = imgpath;
    this->therm_imgpath = thermimgpath;
<<<<<<< HEAD

    cout << "flirImgMetadata::loadMetadata:: Inside flirImgMetadata constructor : " << imgpath <<endl;




    //printReadMetadata();

//    if(exists_test3(therm_imgpath)){
//        cout << "... this file has already been processed " << endl;
//        loadPrereadMetadata();
//        emit signalFlirImgMetadataProgress(10);
//        calcAllMetadata();
//        emit signalFlirImgMetadataProgress(90);
//        checkCalcedMetadata();
//        emit signalFlirImgMetadataProgress(100);
//    }
//    else {
//        readAllMetadata();
//        checkReadMetadata();

//

//        calcAllMetadata();
//        checkCalcedMetadata();

//        writePrereadMetadata();
//        emit signalFlirImgMetadataProgress(100);
//    }
//    printReadMetadata();
//    printCalcedMetadata();
}

void flirImgMetadata::extractRawThermal()
{

    emit signalFlirImgMetadataProgress(80);

    emit signalFlirImgMetadataProgress(95);
}




double flirImgMetadata::readExiftoolTagInfo(std::string tagname)
{
//  std::string readtaglist;

//#ifdef Q_OS_LINUX
//  std::cout << "Linux version" << endl;
//  //  // create our ExifTool objetc
//  //  ExifTool *et = new ExifTool();
//  //  // read metadata from the image
//  //  TagInfo *info = et->ImageInfo(imgpath.c_str(), NULL, 5);

//  //  if (info) {
//  //    for (TagInfo *i=info; i; i=i->next) {
//  //      readtaglist = std::string(i->name);
//  //      if(!ci_find_substr( readtaglist, tagname)){
//  //	//cout << tagname << " : " << i-> value << endl;
//  //        return stod(i->value);
//  //      }
//  //    }
//  //    // we are responsible for deleting the information when done
//  //    delete info;
//  //  } else if (et->LastComplete() <= 0) {
//  //    cerr << "Error executing exiftool!" << endl;
//  //  }
//#endif

//#ifdef Q_OS_WIN32
//  std::cout << "flirImgMetadata::readExiftoolTagInfo:: Windows32 version" << endl;
//  //qDebug() << "App path : " << qApp->applicationDirPath();
//  //windows_exec("exiftool.exe -s -s -s -PlanckR1 .\nabtdimgs\FLIR1097.jpg");
//  double tagvalue = shX->executeTagReaderCmd(tagname);
//  return tagvalue;
//  //std::cout << res << endl;
//#endif

//#ifdef Q_OS_MACX
//  std::cout << "flirImgMetadata::readExiftoolTagInfo:: MacOS X Version " << endl;
//  // double tagvalue = shX->executeTagReaderCmd(tagname);
//  QStringList allTags = shX->extractAllTagsInOneGo();
//  //std::cout << "flirImgMetadata::readExiftoolTagInfo:: " << tagname << " : " << tagvalue << endl;
//  //return tagvalue;
//#endif

//  return 1.0f;
}




=======
    metadataFile = imgpath + std::string(".metadata.txt");
    shX = new shellExecutor(imgpath);
    cout << "Inside flirImgMetadata constructor : " << imgpath <<endl;

    if(exists_test3(therm_imgpath)){
        cout << "... this file has already been processed " << endl;
        loadPrereadMetadata();
        emit signalFlirImgMetadataProgress(10);
        calcAllMetadata();
        emit signalFlirImgMetadataProgress(90);
        checkCalcedMetadata();
        emit signalFlirImgMetadataProgress(100);
    }
    else {
        readAllMetadata();
        checkReadMetadata();

        shX->readyForExtraction(rawthermalimagewidth, rawthermalimageheight);

        extractRawThermal();

        calcAllMetadata();
        checkCalcedMetadata();

        writePrereadMetadata();
        emit signalFlirImgMetadataProgress(100);
    }
}

void flirImgMetadata::readAllMetadata()
{
    readThermalMetadata();
    emit signalFlirImgMetadataProgress(45);
    readBasicMetadata();
    emit signalFlirImgMetadataProgress(65);
}

void flirImgMetadata::extractRawThermal()
{
    shX->executeBinaryExtractionCmd();
    emit signalFlirImgMetadataProgress(80);
    shX->executeConversionCmd();
    emit signalFlirImgMetadataProgress(95);
}

void flirImgMetadata::calcAllMetadata()
{
  calcRaws();
  calcTs();
  calcSes();
}



void flirImgMetadata::calcRaws()
{
  calcRAWmax();
  calcRAWmin();
  calcRAWrefl();
  calcRAWmaxobj();
  calcRAWminobj();
}

void flirImgMetadata::calcTs()
{
  calcTmin();
  calcTmax();
}

void flirImgMetadata::calcSes()
{
  calcSmin();
  calcSmax();
  calcSdelta();
}

void flirImgMetadata::readBasicMetadata()
{
  imagewidth  = readExiftoolTagInfo("ImageWidth"); emit signalFlirImgMetadataProgress(50);
  imageheight = readExiftoolTagInfo("ImageHeight"); emit signalFlirImgMetadataProgress(55);
  rawthermalimagewidth=readExiftoolTagInfo("RawThermalImageWidth"); emit signalFlirImgMetadataProgress(60);
  rawthermalimageheight=readExiftoolTagInfo("RawThermalImageHeight"); emit signalFlirImgMetadataProgress(65);
//    windowsexec7("ImageWidth");
//    windowsexec7("ImageHeight");
//    windowsexec7("RawThermalImageWidth");
//    windowsexec7("RawThermalImageHeight");
}

void flirImgMetadata::readThermalMetadata()
{
  // plancks
  planckr1 = readExiftoolTagInfo("PlanckR1"); emit signalFlirImgMetadataProgress(5);
  planckr2 = readExiftoolTagInfo("PlanckR2"); emit signalFlirImgMetadataProgress(10);
  planckb  = readExiftoolTagInfo("PlanckB");  emit signalFlirImgMetadataProgress(15);
  planckf  = readExiftoolTagInfo("PlanckF");  emit signalFlirImgMetadataProgress(20);
  plancko  = readExiftoolTagInfo("PlanckO");  emit signalFlirImgMetadataProgress(25);

  // tref and emis
  tref     = readExiftoolTagInfo("ReflectedApparentTemperature"); emit signalFlirImgMetadataProgress(30);
  emis     = readExiftoolTagInfo("Emissivity"); emit signalFlirImgMetadataProgress(35);

  // raw values
  rawvaluerange =  readExiftoolTagInfo("RAWvaluerange"); emit signalFlirImgMetadataProgress(40);
  rawvaluemedian = readExiftoolTagInfo("RAWvaluemedian"); emit signalFlirImgMetadataProgress(45);


//  windowsexec7("PlanckR1");
//  windowsexec7("PlanckR2");
//  windowsexec7("PlanckB");
//  windowsexec7("PlanckF");
//  windowsexec7("PlanckO");
//  windowsexec7("RawValueRange");
//  windowsexec7("RawValueMedian");
//  windowsexec7("ReflectedApparentTemperature");
//  windowsexec7("Emissivity");
}


void flirImgMetadata::checkReadMetadata()
{
  assert(planckr1 != 0);
  assert(planckr2 != 0);
  assert(planckb  != 0);
  assert(planckf  != 0);
  assert(plancko  != 0);
  assert(rawvaluemedian != 0);
  assert(rawvaluerange  != 0);
  //assert(tref           != 0);
  assert(emis           != 0);
  //assert(rawrefl        != 0);
}

void flirImgMetadata::checkCalcedMetadata()
{
  assert(rawmax   != 0);
  assert(rawmin   != 0);
  assert(rawmaxobj != 0);
  assert(rawminobj != 0);
  assert(tmin      != 0);
  assert(tmax      != 0);
  assert(smax != 0);
  assert(smin != 0);
  assert(sdelta != 0);

}

void flirImgMetadata::calcRAWmax()
{
  //checkRAWvaluemedian();
  //checkRAWvaluerange();

  rawmax = rawvaluemedian + (rawvaluerange/2);
}

void flirImgMetadata::calcRAWmin()
{
  //checkRAWvaluerange();
  //checkRAWmax();

  rawmin = rawmax - rawvaluerange;
}


// an optional thaaang if tref & RAWrefl are defined
// in the metadata
void flirImgMetadata::calcRAWrefl()
{
  //checkPlancks();
  //checkTref();
  //checkRAWrefl();

  double t11, t12, t13, t14, t15;

  tref = 25;
  t11 = tref + 273.15;
 //cout << "\n t11 " << t11;
  t12 = planckb / t11;
  //cout << "\n t12 " << t12;
  t13 = exp(t12) - planckf;
  //cout << "\n t13 " << t13;
  t14 = planckr2 * t13;
  //cout << "\n t14 " << t14;
  t15 = planckr1 / t14;
  //cout << "\n t15 " << t15;

  rawrefl = t15 - plancko;
  //cout << "\n t16 " << rawrefl << endl;
}

void flirImgMetadata::calcRAWmaxobj()
{
  //checkRAWmax();
  //checkEmis();
  //checkRAWrefl();

  rawmaxobj = (rawmax - (1-emis)*rawrefl)/emis;
}

void flirImgMetadata::calcRAWminobj()
{
  //checkRAWmin();
  //checkEmis();
  //checkRAWrefl();

  rawminobj = (rawmin - (1-emis)*rawrefl)/emis;
}

void flirImgMetadata::calcTmin()
{
  //checkPlancks();
  //checkRAWminobj();
  double t21, t22, t23, t24;

  t21 = (rawminobj + plancko);
  t22 = planckr2 * t21;
  t23 = (planckr1/t22) + planckf;
  t24 = planckb / log(t23);
  tmin = t24 - 273.15;
}

void flirImgMetadata::calcTmax()
{
  //checkPlancks();
  //checkRAWmaxobj();
  double t31, t32, t33, t34;

  t31 = (rawmaxobj + plancko);
  t32 = planckr2 * t31;
  t33 = (planckr1/t32) + planckf;
  t34 = planckb / log(t33);
  tmax = t34 - 273.15;
}


void flirImgMetadata::calcSmax()
{
  //checkPlancks();
  //checkRAWmax();
  double t41, t42, t43;

  t41 = planckr2 * (rawmax + plancko);
  //cout << " Smax t41 : " << t41 << endl;
  t42 = ((planckr1/t41) + planckf);
  //cout << " Smax t42 : " << t42 << endl;
  t43 = log(t42);
  //cout << " Smax t43 : " << t43 << endl;

  smax = planckb / t43;
}

void flirImgMetadata::calcSmin()
{
  //checkPlancks();
  //checkRAWmin();
  //double planckr1, planckr2;
  //double planckb, planckf, plancko;
  //double rawmin;
  //double Smin;
  double t51, t52, t53;

  t51 = planckr2 * (rawmin + plancko);
  //cout << " Smin t51 : " << t51 << endl;
  t52 = ((planckr1/t51) + planckf);
  //cout << " Smin t52 : " << t52 << endl;
  t53 = log(t52);
  //cout << " Smin t53 : " << t53 << endl;

  smin = planckb / t53;
  //cout << " Smin    : " << Smin << endl;
}

void flirImgMetadata::calcSdelta()
{
  //checkSmax();
  //checkSmin();

  sdelta = smax - smin;
}

double flirImgMetadata::calcTempForOnePixel(double thermalintensityvalue)
{
  double t61, t62, t63, t64, t65, t66, t67;
  // just linearly mapped the thermal intensity values to a value between
  // rawminobj and rawmaxobj using
  // https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another

  //cout << "ThermalIntensityValue " << thermalintensityvalue;
  t61 = rawminobj + ((rawmaxobj-rawminobj)/(65535))*(thermalintensityvalue);
  // cout << "t61 : " << t61 << endl;
  //t61 = 13900;
  t62 = t61 + plancko;
  t63 = planckr2 * t62;
  t64 = (planckr1/t63) + planckf;
  //t65 = planckb/log(t64) - smin;
  t65 = planckb/log(t64);
  //t66 = t65/sdelta;
  t66 = t65 - 273.15;

  return t66;
}

#include <QtGlobal>
#include "exec.h"
double flirImgMetadata::readExiftoolTagInfo(std::string tagname)
{
  std::string readtaglist;

//  // create our ExifTool objetc
//  ExifTool *et = new ExifTool();
//  // read metadata from the image
//  TagInfo *info = et->ImageInfo(imgpath.c_str(), NULL, 5);

//  if (info) {
//    for (TagInfo *i=info; i; i=i->next) {
//      readtaglist = std::string(i->name);
//      if(!ci_find_substr( readtaglist, tagname)){
//	//cout << tagname << " : " << i-> value << endl;
//        return stod(i->value);
//      }
//    }
//    // we are responsible for deleting the information when done
//    delete info;
//  } else if (et->LastComplete() <= 0) {
//    cerr << "Error executing exiftool!" << endl;
//  }



#ifdef Q_OS_LINUX
  //std::cout << "Linux version" << endl;
#endif

#ifdef Q_OS_WIN
  //std::cout << "Windows version" << endl;
#endif

#ifdef Q_OS_WIN32
  //std::cout << "Windows32 version" << endl;
  //qDebug() << "App path : " << qApp->applicationDirPath();
  //windows_exec("exiftool.exe -s -s -s -PlanckR1 .\nabtdimgs\FLIR1097.jpg");
  double tagvalue = shX->executeTagReaderCmd(tagname);
  return tagvalue;
  //std::cout << res << endl;
#endif

#ifdef Q_OS_WIN64
  std::cout << "Windows64 version" << endl;
#endif

#ifdef Q_OS_MSDOS
  std::cout << "DOS version" << endl;
#endif

  return 1.0f;
}

void flirImgMetadata::printCalcedMetadata()
{
  cout << "\n\n\nRaws calculated" << endl;
  cout << "RAWmax         : "     << rawmax         << endl;
  cout << "RAWmin         : "     << rawmin         << endl;
  cout << "RAWrefl        : "     << rawrefl        << endl;
  cout << "RAWmaxobj      : "     << rawmaxobj      << endl;
  cout << "RAWminobj      : "     << rawminobj      << endl;

  cout << "\n\n Tmin, Tmax ..." << endl;
  cout << "Tmin    : "          << tmin   << endl;
  cout << "Tmax    : "          << tmax   << endl;

  cout << "\n\n Smax, Smin, Sdelta calculated ..." << endl;
  cout << "Smax    : "          << smax   << endl;
  cout << "Smin    : "          << smin   << endl;
  cout << "Sdelta  : "          << sdelta << endl;

}

void flirImgMetadata::printReadMetadata()
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


void flirImgMetadata::printImageSummary()
{
  //cout << "\n\n\nExiftags read .... "            << endl;
  cout << " | PlanckR1       : " << setw(10) << planckr1
       << " | RAWmax         : " << setw(10) << rawmax
       << " | Tmin    : "        << setw(10) << tmin
       << " | Smax    : "        << setw(10) << smax   << endl;
  cout << " | PlanckR2       : " << setw(10) << planckr2
       << " | RAWmin         : " << setw(10) << rawmin
       << " | Tmax    : "        << setw(10) << tmax
       << " | Smin    : "        << setw(10) << smin   << endl;
  cout << " | PlanckB        : " << setw(10) << planckb
       << " | RAWrefl        : " << setw(10) << rawrefl
       << setw(23) << ""
       << " | Sdelta   : " << setw(10) << sdelta << endl;
  cout << " | PlanckF        : " << setw(10) << planckf
       << " | RAWmaxobj      : " << setw(10) << rawmaxobj << endl;
  cout << " | PlanckO        : " << setw(10) << plancko
       << " | RAWminobj      : " << setw(10) << rawminobj << endl;
  cout << " | Tref           : " << setw(10) << tref           << endl;
  cout << " | Emis           : " << setw(10) << emis           << endl;
  cout << " | RawValueMedian : " << setw(10) << rawvaluemedian << endl;
  cout << " | RawValueRange  : " << setw(10) << rawvaluerange  << endl;
}

void flirImgMetadata::loadPrereadMetadata()
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

void flirImgMetadata::writePrereadMetadata()
{
    std::ofstream myfile;
    myfile.open(this->metadataFile);
    cout << " WRITING THE METADATA " << endl;

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
    else cerr<<"Unable to open file " << this->metadataFile <<endl;
}
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
