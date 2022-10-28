#include "flirimg.h"
#include <QDebug>
#include <QProcess>

bool flirImg::executeRepaletteImage()
{
    return true;
}



void flirImg::executeZeCommand(QString cmdPath, QStringList cmdArgs, QString &result, QString &errors, cmdExecType t)
{
    bool    exec_success;
    QProcess cmdProcess;

    QString fullCmd = cmdPath + " " + cmdArgs.join(" ");
    qDebug() << "flirImg::executeZeCmd trying to execute : " << fullCmd;
#ifdef Q_OS_WIN32
    //qDebug()<<"flirImg::executeZeCommand -- works here";
    cmdProcess.start(fullCmd);
#endif
#ifdef Q_OS_UNIX
    if(t == execute)
        cmdProcess.execute(cmdPath, cmdArgs);
    else if(t == start)
        cmdProcess.start(cmdPath, cmdArgs);
#endif
    exec_success = cmdProcess.waitForFinished();

    result = cmdProcess.readAllStandardOutput();
    errors = cmdProcess.readAllStandardError();
    if(exec_success){
        //qDebug() << "flirImg::executeZeCommand - Size of result is " << result.size();
        //qDebug() << "flirImg::executeZeCommand - Conversion result : " << result;
    } else{
        //qDebug() << "flirImg::executeZeCommand - Size of errors is " << errors.size();
        //qDebug() << "flirImg::executeZeCommand - Conversion errors : " << errors;
    }

    qDebug() << "flirImg::executeZeCmd -- Finished";
    cmdProcess.close();
}


QStringList flirImg::buildExtractionArgList(extractArgTypes argType)
{
    QStringList argList;
    switch(argType)
    {
        case ALLTAGS:
            argList = QStringList() << "-a"
                                    << imgpath;
            break;
        case UNORDEREDRAWS:
            argList = QStringList() << imgpath
                                    << "-RawThermalImage" << "-b"
                                    << "-W" << unorderedRAWs_imgpath;
            break;
        case PALETTE:
            argList = QStringList() << imgpath
                                    << "-b -Palette"
                                    << "-W" << rawpalette_imgpath;
    }
    return argList;
}

QStringList flirImg::buildConvertArgList(convertArgTypes argType)
{
    QStringList argList;
    switch(argType)
    {
        case ORDEREDRAWS_2_DISPLAYPNG :
            transformToWindowsPath(orderedRAWs_imgpath);
            //QString sizeStr1 = QString::number(rawthermalimagewidth)+"X"+QString::number(rawthermalimageheight);
            //QString sizeStr1 = QString::number(rawthermalimageheight);
            argList = QStringList()
                      << orderedRAWs_imgpath
                      //<< "-size"
                      //<< "640x480"
                      << displayOrderedRAWs_imgpath;
            break;
        case UNORDEREDRAWS_2_DISPLAYPNG :
            transformToWindowsPath(unorderedRAWs_imgpath);
            argList = QStringList()
                      << unorderedRAWs_imgpath
                      << displayUnorderedRAWs_imgpath;
            break;
        case RAWPALETTE_2_1DPALETTE:
            QString sizeStr2 = QString::number(paletteColors) + "X1";
            argList = QStringList()
                      << rawpalette_imgpath
                      << "-size"
                      << sizeStr2
                      << "-depth 8"
                      << "YCbCr:- -separate -swap 1,2 -set colorspace YCbCr -combine -colorspace RGB -auto-level"
                      << palette_imgpath;


    }
    return argList;
}


bool flirImg::convertOrderedRAWsToDisplayPNG()
{
    QString result, errors;
    executeZeCommand(getConvertPath(), buildConvertArgList(ORDEREDRAWS_2_DISPLAYPNG), result, errors, execute);
    return doesFileExist(displayOrderedRAWs_imgpath);
            //canReadFileIntoMat(displayOrderedRAWs_imgpath, displayOrderedRAWsMat);
}

bool flirImg::convertUnorderedRAWsToDisplayPNG()
{
    QString result, errors;
    executeZeCommand(getConvertPath(), buildConvertArgList(UNORDEREDRAWS_2_DISPLAYPNG), result, errors, execute);
    return doesFileExist(displayOrderedRAWs_imgpath);
            //canReadFileIntoMat(displayUnorderedRAWs_imgpath, displayUnorderedRAWsMat);
}


#include <QDir>
QString flirImg::getExiftoolPath()
{
    QString exiftoolPath;
#ifdef Q_OS_WIN32
    exiftoolPath = QDir::currentPath() + "/nabtdimgs/exiftool";
#endif
#ifdef Q_OS_MACX
    exiftoolPath = "/usr/local/bin/exiftool";
#endif
#ifdef Q_OS_LINUX
    exiftoolPath = "/usr/bin/exiftool";
#endif
    return exiftoolPath;
}

QString flirImg::transformToWindowsPath(QString str)
{
    QString transformedPath;
#ifdef Q_OS_WIN32
    std::replace(str.begin(), str.end(),
                 QChar('/'), QChar('\\'));
#endif
    return str;
}

QString flirImg::getConvertPath()
{
    QString convertPath;
#ifdef Q_OS_WIN32
    convertPath = QDir::currentPath() + "/nabtdimgs/convert";
    convertPath = transformToWindowsPath(convertPath);
    // convertPath = "C:/opencvqt5/flirbaba-gui/imagemagick/convert does not work
    // gives an error like Unrecognized option -depth 16
#endif
#ifdef Q_OS_MACX
    convertPath  = "/usr/local/bin/convert";
#endif
#ifdef Q_OS_LINUX
    convertPath  = "/usr/local/bin/convert";
#endif
    return convertPath;
}
