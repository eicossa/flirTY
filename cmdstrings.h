#ifndef CMDSTRINGS_H
#define CMDSTRINGS_H

#include <iostream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <QStringList>
#include <QProcess>
#include <QDebug>
#include <QDir>

using namespace std;

class toolPathStrings{
    QString exiftoolPath;
    QString convertPath;
public:
    toolPathStrings()
    {
#ifdef Q_OS_WIN32
        QString currPath = QDir::currentPath();
        exiftoolPath = currPath + "/nabtdimgs/exiftool";
        qDebug() << "toolPathStrings::toolPathStrings - path of exiftool : "
                 << exiftoolPath;
        //convertPath  = "C:\\opencvqt5\\flirbaba-gui\\imagemagick\\convert";
        //convertPath = "C:\\opencvqt5\\build-tabbedFlirbabaGuiSegmentor-Desktop_Qt_5_9_0_MinGW_32bit-Release\\nabtdimgs\\convert";
        convertPath  = currPath + "/nabtdimgs/convert";
        std::replace( convertPath.begin(), convertPath.end(),
                      QChar('/'), QChar('\\') );
        qDebug() << "ConvertPath normalized : " << convertPath;
        // convertPath = "C:/opencvqt5/flirbaba-gui/imagemagick/convert does not work
        // gives an error like Unrecognized option -depth 16
#endif
#ifdef Q_OS_MACX
      exiftoolPath = "/usr/local/bin/exiftool";
      convertPath  = "/usr/local/bin/convert";
#endif
#ifdef Q_OS_LINUX
      exiftoolPath = "/usr/bin/exiftool";
      convertPath  = "/usr/local/bin/convert";
#endif
    }

    QString getExiftoolPath()
    {
      return exiftoolPath;
    }

    QString getConvertPath()
    {
      return convertPath;
    }    
};



class metadataReaderStrings{
  std::string imgpath;
public:
  metadataReaderStrings(std::string imgpath)
  {
      this->imgpath = imgpath;
  }

  QStringList allTagsArgs()
  {
      QString iPath          = QString::fromStdString(imgpath);
      QStringList exifParams = QStringList() << "-a"
                                             << iPath;
      qDebug() << "metadataReaderStrings::allTagsArgs to be executed " << exifParams;
      return exifParams;
  }
};


class extractionStrings{
  // Equivalent commandline incantation :
  // exiftool FLIRjpgfilename.jpg -b -RawThermalImage FLIRjpgfilename.bin
  QString imgpath;
  QString preOrderedRAWs_imgpath;
  QString therm_imgpath;

  int         rawthermalimgwidth;
  int         rawthermalimgheight;

public:
  extractionStrings(std::string fimgpath, int rtWidth, int rtHeight, std::string preorderedRAWs_imgpath)
  {
    this->imgpath = QString::fromStdString(fimgpath);
    this->preOrderedRAWs_imgpath = QString::fromStdString(preorderedRAWs_imgpath);
    //this->therm_imgpath = imgpath + ".thermout.png";

    this->rawthermalimgwidth = rtWidth;
    this->rawthermalimgheight = rtHeight;
  }



  QStringList getRawThermalImageExtractionArgs()
  {
      QStringList strlist = QStringList()
              << imgpath
              << "-RawThermalImage"
              << "-b"
              << "-W"
              << preOrderedRAWs_imgpath;

      //qDebug() << "binary imgpath " << QString::fromStdString(bin_imgpath);

      return strlist;
  }

};

// convert - gray:- |
// convert -depth 16 -endian lsb -size 640x480 gray:- Outputfilename.png
class conversionStrings{
    int         rawthermalimgwidth;
    int         rawthermalimgheight;
    QString     imgpath;
    QString     unorderedRAWs_imgpath;
    QString     displayRAWs_imgpath;

    QString     conversionBinaryImgpath;
    QString     conversionRAWsResizedImgpath;

//    QStringList strlist = QStringList()
//            << "convert "
//            << therm_imgpath
//            << "-fx "
//            << (log(planckr1/(planckr2*(65535*u + plancko)) + planckf) - Smin)/Sdelta
//            << irgrayscale_imgpath;

public:
  conversionStrings(std::string fimgpath,
                    int rtWidth, int rtHeight,
                    std::string unorderedRAWs_imgpath,
                    std::string displayRAWs_imgpath)
  {
    this->imgpath               = QString::fromStdString(fimgpath);
    this->unorderedRAWs_imgpath = QString::fromStdString(unorderedRAWs_imgpath);
    this->displayRAWs_imgpath   = QString::fromStdString(displayRAWs_imgpath);


    this->rawthermalimgwidth    = rtWidth;
    this->rawthermalimgheight   = rtHeight;

    qDebug() << "conversionStrings::convert216bitGrayscaleArgs - About to pass #1";

#ifdef Q_OS_WIN32
      std::replace(unorderedRAWs_imgpath.begin(), unordered_RAWs_imgpath.end(),
                   QChar('/'), QChar('\\'));
      std::replace(unorderedRAWs_imgpath.begin(), unordered_RAWs_imgpath.end(),
                   QChar('/'), QChar('\\'));
#endif
  }

    QStringList convert216bitGrayscaleArgs()
    {
        QString sizestr = "-size "
                + QString::fromStdString(std::to_string(rawthermalimgwidth))
                + "x"
                + QString::fromStdString(std::to_string(rawthermalimgheight));
        qDebug() << "conversionStrings::convert216bitGrayscaleArgs - Size String" << sizestr;

        QStringList strlist = QStringList()
                << conversionBinaryImgpath
                << "-depth 16"
        //        << "-endian lsb"
        //        << "-auto-level"
        //        << "-colorspace gray"
                << sizestr
                << conversionRAWsResizedImgpath;
        qDebug() << "conversionStrings::convert216bitGrayscaleArgs - Prepared the string" << strlist;
        return strlist;
    }

    QStringList getConvertRAW2GrayscalePNGArgs()
    {
        QStringList strList = QStringList()
                << unorderedRAWs_imgpath
                << displayRAWs_imgpath;
        return strList;
    }
};


#endif // CMDSTRINGS_H
