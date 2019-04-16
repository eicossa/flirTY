#include "cmdstrings.h"
#include <QDebug>




//QStringList cmdStrings::getConvert2Grayscale16bitCmdString()
//{
////    std::string cmdString;
////    int rawthermalimagewidth  = fimg->getMetadata()->getRAWThermalImageWidth();
////    int rawthermalimageheight = fimg->getMetadata()->getRAWThermalImageHeight();

////    QString sizestr = "-size "
////            + QString::fromStdString(std::to_string(rawthermalimagewidth))
////            + "x"
////            + QString::fromStdString(std::to_string(rawthermalimageheight));
////    qDebug() << "size string" << sizestr;

////    QStringList strlist = QStringList()
////            << QString::fromStdString(fimg->getBinaryImgPath())
////            << "-depth 16"
////            << "-endian msb"
////            << "-auto-level"
////            << "-colorspace gray"
////            << sizestr
////            << QString::fromStdString(fimg->getThermalImgPath());
////    return strlist;
//}

//QStringList cmdStrings::getExtractThermalCmdString()
//{
////    std::string cmdString;
////    std::string orig_imgpath, bin_imgpath, therm_imgpath;

////    orig_imgpath = fimg->getImgPath();
////    bin_imgpath  = fimg->getBinaryImgPath();
////    therm_imgpath = fimg->getThermalImgPath();

////    QStringList strlist = QStringList()
////            << QString::fromStdString(orig_imgpath)
////            << "-RawThermalImage"
////            << "-b"
////            << "-W"
////            << QString::fromStdString(bin_imgpath);

////    qDebug() << "binary imgpath " << QString::fromStdString(bin_imgpath);

////    return strlist;
//}

//void cmdStrings::convertBinary2GrayscaleWindows()
//{
////    QProcess convertProcess;
////    //convertProcess.setWorkingDirectory("C:/opencvqt5/flirbaba-gui/imagemagick/");
////    //convertProcess.start("C:/opencvqt5/flirbaba-gui/imagemagick/convert.exe", arglist);
////    //convertProcess.start("cmd.exe", arglist);

////    QString pgm   = "C:/opencvqt5/flirbaba-gui/imagemagick/convert";
////    QStringList convert2grayscale16bitCmdStringlist;
////    convert2grayscale16bitCmdStringlist = getConvert2Grayscale16bitCmdString();

////    QString arg = convert2grayscale16bitCmdStringlist.join(" ");

////    QString fullString = pgm + " " + arg;
////    qDebug() << "conversion string : "<< fullString;

////    std::string fstring = fullString.toStdString();
////    const char* fstr;
////    fstr = fstring.c_str();

////    //system(fstr);
////    convertProcess.start(fstr);

////    QString errors, result;

////    if(convertProcess.waitForFinished()){
////        errors = convertProcess.readAllStandardError();
////        result = convertProcess.readAllStandardOutput();
////        qDebug() << " Conversion errors : " << errors;
////        qDebug() << " Extraction result : " << result;
////    }
////    convertProcess.close();
//}

//double cmdStrings::exiftoolExtractTagWindows(std::string tagname)
//{
////   // "-PlanckR1"
////   // "-PlanckR2"
////   // "-PlanckB"
////   // "-PlanckF"
////   // "-PlanckO"
////   // "-RawValueRange"
////   // "-RawValueMedian"
////   // "-ReflectedApparentTemperature"
////   // "-Emissivity"

////    double   tagvalue;
////    QProcess exifProcess;
//////    QString tagflag = "-" + QString::fromStdString(tagname);

//////    QStringList exifParams = QStringList() << "-s -S"
//////                                           << tagflag
//////                                           << "C:\\opencvqt5\\flirbaba-gui\\nabtdimgs\\FLIR1097.jpg";

////    exifProcess.start("C:\\opencvqt5\\flirbaba-gui\\exiftool", exifParams);
////    QString errors, result;

////    if(exifProcess.waitForReadyRead(2000)){
////      errors = exifProcess.readAllStandardError();
////      //qDebug() << "Errors : " << errors;
////      result = exifProcess.readAllStandardOutput();
////      //qDebug() << "Output : " << result;

////      QStringList list1 = result.split(':');

////      tagvalue = list1.at(1).toDouble();
////      qDebug() << tagvalue;
////    }

////    exifProcess.close();
////    return tagvalue;
//}

//void cmdStrings::exiftoolExtractThermalWindows()
//{
////    QProcess exifProcess;
////    QStringList extractThermalImageCmdStringlist;
////    extractThermalImageCmdStringlist    = getExtractThermalCmdString();
////    QString exiftoolPath = getExiftoolPath();
////    exifProcess.execute(exiftoolPath, extractThermalImageCmdStringlist);

////    QString errors, result;
////    qDebug() << "Thermal extraction conversion string : " << extractThermalImageCmdStringlist.join(" ");

//////    if(exifProcess.waitForReadyRead(2000)){
//////      errors = exifProcess.readAllStandardError();
//////      result = exifProcess.readAllStandardOutput();
//////      qDebug() << "Extraction Errors : " << errors;
//////      qDebug() << "Extraction Output : " << result;
//////    }


//////    if (!exifProcess.waitForStarted()){
//////      qDebug() << "Not yet started extraction ";
//////    }


////    //exifProcess.write("Qt rocks!");
////    //exifProcess.closeWriteChannel();

//////    if (!exifProcess.waitForFinished()){
//////      qDebug() << "Extraction done";
//////    }
////    //exifProcess.close();

////    //qDebug() << "Output : " << exifProcess.readAll();
////    qDebug() << "Length of output " << exifProcess.readAll().length();
//}
