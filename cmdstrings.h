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
<<<<<<< HEAD
#ifdef Q_OS_WIN32
        QString currPath = QDir::currentPath();
        exiftoolPath = currPath + "/nabtdimgs/exiftool";
        qDebug() << "toolPathStrings::toolPathStrings - path of exiftool : "
                 << exiftoolPath;
=======
        QString currPath = QDir::currentPath();
        exiftoolPath = currPath + "/nabtdimgs/exiftool";
        qDebug() << "ExiftoolPath : " << exiftoolPath;
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
        //convertPath  = "C:\\opencvqt5\\flirbaba-gui\\imagemagick\\convert";
        //convertPath = "C:\\opencvqt5\\build-tabbedFlirbabaGuiSegmentor-Desktop_Qt_5_9_0_MinGW_32bit-Release\\nabtdimgs\\convert";
        convertPath  = currPath + "/nabtdimgs/convert";
        std::replace( convertPath.begin(), convertPath.end(),
                      QChar('/'), QChar('\\') );
        qDebug() << "ConvertPath normalized : " << convertPath;
        // convertPath = "C:/opencvqt5/flirbaba-gui/imagemagick/convert does not work
        // gives an error like Unrecognized option -depth 16
<<<<<<< HEAD
#endif
#ifdef Q_OS_MACX
      exiftoolPath = "/usr/local/bin/exiftool";
      convertPath  = "/usr/local/bin/convert";
#endif
#ifdef Q_OS_LINUX
      exiftoolPath = "/usr/bin/exiftool";
      convertPath  = "/usr/local/bin/convert";
#endif
=======
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
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
<<<<<<< HEAD
  {
      this->imgpath = imgpath;
  }

  QStringList allTagsArgs()
  {
      QString iPath          = QString::fromStdString(imgpath);
      QStringList exifParams = QStringList() << "-a"
                                             << iPath;
      qDebug() << "metadataReaderStrings::allTagsArgs to be executed " << exifParams;
=======
  {  this->imgpath = imgpath; }
  
  QStringList tagReaderArgs(std::string tagname)
  {
      QString tagflag = "-" + QString::fromStdString(tagname);
      QString iPath   = QString::fromStdString(imgpath);
      QStringList exifParams = QStringList() << "-s -S"
                                             << tagflag
                                             << iPath;
      qDebug() << "Inside mdRS : " + iPath;
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
      return exifParams;
  }
};


class extractionStrings{
<<<<<<< HEAD
  // Equivalent commandline incantation :
  // exiftool FLIRjpgfilename.jpg -b -RawThermalImage FLIRjpgfilename.bin
  QString imgpath;
  QString preOrderedRAWs_imgpath;
=======
  QString imgpath;
  QString binar_imgpath;
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
  QString therm_imgpath;

  int         rawthermalimgwidth;
  int         rawthermalimgheight;

public:
<<<<<<< HEAD
  extractionStrings(std::string fimgpath, int rtWidth, int rtHeight, std::string preorderedRAWs_imgpath)
  {
    this->imgpath = QString::fromStdString(fimgpath);
    this->preOrderedRAWs_imgpath = QString::fromStdString(preorderedRAWs_imgpath);
    //this->therm_imgpath = imgpath + ".thermout.png";
=======
  extractionStrings(std::string fimgpath, int rtWidth, int rtHeight)
  {
    this->imgpath = QString::fromStdString(fimgpath);
    this->binar_imgpath = imgpath + ".bin";
    this->therm_imgpath = imgpath + ".thermout.png";
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

    this->rawthermalimgwidth = rtWidth;
    this->rawthermalimgheight = rtHeight;
  }

<<<<<<< HEAD


  QStringList getRawThermalImageExtractionArgs()
=======
  QStringList getConversionArgs()
  {
      QString sizestr = "-size "
              + QString::fromStdString(std::to_string(rawthermalimgwidth))
              + "x"
              + QString::fromStdString(std::to_string(rawthermalimgheight));
      qDebug() << "size string" << sizestr;

      QString conversionBinaryImgpath, conversionThermlImgpath;
      conversionBinaryImgpath = binar_imgpath;
      conversionThermlImgpath = therm_imgpath;

      std::replace(conversionBinaryImgpath.begin(), conversionBinaryImgpath.end(),
                   QChar('/'), QChar('\\'));
      std::replace(conversionThermlImgpath.begin(), conversionThermlImgpath.end(),
                   QChar('/'), QChar('\\'));



      QStringList strlist = QStringList()
              << conversionBinaryImgpath
              << "-depth 16"
              << "-endian msb"
              << "-auto-level"
              << "-colorspace gray"
              << sizestr
              << conversionThermlImgpath;
      return strlist;
  }


  QStringList getExtractionArgs()
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
  {
      QStringList strlist = QStringList()
              << imgpath
              << "-RawThermalImage"
              << "-b"
              << "-W"
<<<<<<< HEAD
              << preOrderedRAWs_imgpath;
=======
              << binar_imgpath;
>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3

      //qDebug() << "binary imgpath " << QString::fromStdString(bin_imgpath);

      return strlist;
  }

};

<<<<<<< HEAD
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


=======

class shellExecutor{
    std::string           imgpath;
    toolPathStrings       *tPS;
    metadataReaderStrings *mdRS;
    extractionStrings     *eS;

public:
    shellExecutor(std::string fimgpath)
    {
      imgpath       = fimgpath;
      tPS           = new toolPathStrings();
      mdRS          = new metadataReaderStrings(imgpath);
      qDebug() << "shellExecutor initialized with " + QString::fromStdString(imgpath);
      //es = new extractionStrings();
    }

    void readyForExtraction(int rtWidth, int rtHeight)
    {
        eS = new extractionStrings(imgpath, rtWidth, rtHeight);
    }

    // "-PlanckR1"
    // "-PlanckR2"
    // "-PlanckB"
    // "-PlanckF"
    // "-PlanckO"
    // "-RawValueRange"
    // "-RawValueMedian"
    // "-ReflectedApparentTemperature"
    // "-Emissivity"
    double executeTagReaderCmd(std::string tagname)
    {
        double   tagvalue;
        QProcess tagReaderProcess;
        QString exiftoolPath = tPS->getExiftoolPath();
        QStringList tagReaderArgs = mdRS->tagReaderArgs(tagname);
        qDebug() << tagReaderArgs;
        tagReaderProcess.start(exiftoolPath, tagReaderArgs);

        QString errors, result;
    
        if(tagReaderProcess.waitForFinished()){
          errors = tagReaderProcess.readAllStandardError();
          //qDebug() << "Errors : " << errors;
          result = tagReaderProcess.readAllStandardOutput();
          //qDebug() << "Output : " << result;
    
          QStringList list1 = result.split(':');
    
          tagvalue = list1.at(1).toDouble();
          qDebug() << tagvalue;
        }
    
        tagReaderProcess.close();
        return tagvalue;
    }

    bool executeBinaryExtractionCmd()
    {
        if(eS == NULL){
            qDebug() << "Extraction called without adequate prep ... Aborting ... ";
            exit(1);
        }

        bool exec_success;
        QProcess extractProcess;
        QString exiftoolPath = tPS->getExiftoolPath();
        QStringList extractionArgs = eS->getExtractionArgs();

        QString errors, result;
        extractProcess.execute(exiftoolPath, extractionArgs);

        exec_success = extractProcess.waitForFinished();

        if(exec_success){
            result = extractProcess.readAllStandardOutput();
            qDebug() << "Extraction result : " << result;
        } else {
            errors = extractProcess.readAllStandardError();
            qDebug() << "Extraction errors : " << errors;
        }

        extractProcess.close();
        return exec_success;
    }

    bool executeConversionCmd()
    {
        if(eS == NULL){
            qDebug() << "Extraction called without adequate prep ... Aborting ... ";
            exit(1);
        }

        bool exec_success;
        QProcess convertProcess;
        QString convertPath   = tPS->getConvertPath();
        QStringList conversionArgs = eS->getConversionArgs();

        QString errors, result;
        //convertProcess.execute(convertPath, conversionArgs);
        //qDebug() << "Conversion string " << convertPath << conversionArgs.join(" ");
        QString fullCmd = convertPath + " " + conversionArgs.join(" ");
        qDebug() << "Full Convert program : " << fullCmd;
        convertProcess.execute(fullCmd);

        exec_success = convertProcess.waitForFinished();

        if(exec_success){
            result = convertProcess.readAllStandardOutput();
            qDebug() << "Conversion result : " << result;
        } else {
            errors = convertProcess.readAllStandardError();
            qDebug() << "Conversion errors : " << errors;
        }

        convertProcess.close();
        return exec_success;
    }
};

>>>>>>> ae9bc093c8864ebd3392746a0ea24e22d6c9cbd3
#endif // CMDSTRINGS_H
