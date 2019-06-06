#include "flirimgmetadata.h"

#include <QDebug>
#include <QtGlobal>
#include "exec.h"




flirImgMetadata::flirImgMetadata()
{
  imgpath = "";
}

void flirImgMetadata::loadMetadata(std::string imgpath, std::string thermimgpath)
{
    this->imgpath       = imgpath;
    this->therm_imgpath = thermimgpath;

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




