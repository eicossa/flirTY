#include "shellexecutor.h"



shellExecutor::shellExecutor(std::string fimgpath,
                             std::string unorderedRAWs_imgpath,
                             std::string displayRAWs_imgpath)
{
  imgpath       = fimgpath;
  tPS           = new toolPathStrings();
  mdRS          = new metadataReaderStrings(imgpath);


  qDebug() << "shellExecutor initialized with " + QString::fromStdString(imgpath);
  this->unorderedRAWs_imgpath = unorderedRAWs_imgpath;
  this->displayRAWs_imgpath = displayRAWs_imgpath;

  //es = new extractionStrings();
}

void shellExecutor::readyForExtraction(int rtWidth, int rtHeight)
{
    eS = new extractionStrings(imgpath, rtWidth, rtHeight, unorderedRAWs_imgpath);
    cS = new conversionStrings(imgpath,
                               rtWidth, rtHeight,
                               unorderedRAWs_imgpath,
                               displayRAWs_imgpath);
}

//double shellExecutor::extractOneTag(std::string tagname)
//{
//    double   tagvalue;
//    QProcess tagReaderProcess;
//    QString exiftoolPath = tPS->getExiftoolPath();
//    QStringList tagReaderArgs = mdRS->tagReaderArgs(tagname);

//    qDebug() << "shellExecutor::executeTagReaderCmd : " << tagReaderArgs;
//    tagReaderProcess.start(exiftoolPath, tagReaderArgs);

//    QString errors, result;

//    if(tagReaderProcess.waitForFinished()){
//      errors = tagReaderProcess.readAllStandardError();
//      //qDebug() << "Errors : " << errors;
//      result = tagReaderProcess.readAllStandardOutput();
//      //qDebug() << "Output : " << result;

//      QStringList list1 = result.split(':');

//      tagvalue = list1.at(1).toDouble();
//      qDebug() << tagvalue;
//    }

//    tagReaderProcess.close();
//    return tagvalue;
//}

QStringList shellExecutor::extractAllTagsInOneGo()
{
    qDebug() << "shellExecutor::extractAllFlirTagsInOneGo : Are we even here ? ";
    QProcess tagReaderProcess;
    QString     exiftoolPath  = tPS->getExiftoolPath();
    //QStringList tagReaderArgs = mdRS->allFlirTagsArgs();
    QStringList tagReaderArgs = mdRS->allTagsArgs();

    qDebug() << "shellExecutor::extractAllFlirTagsInOneGo : " << exiftoolPath << tagReaderArgs;
    //tagReaderProcess.start(exiftoolPath, tagReaderArgs);

    QString fullCmd = exiftoolPath + " " + tagReaderArgs.join(" ");
    qDebug() << "shellExecutor::extractAllFlirTagsInOneGo : " << fullCmd;
    QString errors, result;
    tagReaderProcess.start(fullCmd);

    if(tagReaderProcess.waitForFinished()){
      errors = tagReaderProcess.readAllStandardError();
      //qDebug() << "extractAllFlirTagsInOneGo::Errors : " << errors;
      result = tagReaderProcess.readAllStandardOutput();
      //qDebug() << "extractAllFlirTagsInOneGo::Output : " << result;

      //QStringList list1 = result.split(':');

      //tagvalue = list1.at(1).toDouble();
      //qDebug() << tagvalue;
    }else{
        qDebug() << "shellExecutor::extractAllFlirTagsInOneGo - All didn't work : " << tagReaderProcess.readAll();
    }

    QStringList allFlirTags = result.split('\n');
    for ( const auto& i : allFlirTags  )
    {
        // qDebug() << "shellExecutor::extractAllTagsInOneGo - " << i;
    }

    tagReaderProcess.close();

    return allFlirTags;
}


bool shellExecutor::executeRAWsExtractionCmd()
{
    if(eS == NULL){
        qDebug() << "Extraction called without adequate prep ... Aborting ... ";
        exit(1);
    }

    bool exec_success;
    QProcess extractProcess;
    QString exiftoolPath = tPS->getExiftoolPath();
    QStringList extractionArgs = eS->getRawThermalImageExtractionArgs();

    QString errors, result;
    extractProcess.execute(exiftoolPath, extractionArgs);
    exec_success = extractProcess.waitForFinished();
    qDebug() << "shellExector::executeRAWsExtractionCmd - Attempting to run cmd " << exiftoolPath << extractionArgs;

    if(exec_success){
        result = extractProcess.readAllStandardOutput();
        qDebug() << "Extraction result : " << result;
    } else {
        errors = extractProcess.readAllStandardError();
        qDebug() << "Extraction errors : " << errors;
    }

    extractProcess.close();
    qDebug() << "shellExecutor::executeRAWsExtractionCmd - Done";
    return exec_success;
}

//bool shellExecutor::executeRAWresizingCmd()
//{
//    if(cS == NULL){
//        qDebug() << " shellExecutor::executeConversionCmd: Extraction called without adequate prep ... Aborting ... ";
//        exit(1);
//    }

//    bool exec_success;
//    QProcess convertProcess;
//    QString convertPath        = tPS->getConvertPath();
//    QStringList conversionArgs = cS->convert216bitGrayscaleArgs();
//    qDebug() << "shellExecutor::executeRAWresizingCmd - What ?";

//    QString errors, result;
//    //convertProcess.execute(convertPath, conversionArgs);
//    qDebug() << "shellExecutor::executeRAWresizingCmd - Conversion string "
//             << convertPath << conversionArgs.join(" ");
//    QString fullCmd = convertPath + " " + conversionArgs.join(" ");
//    qDebug() << "shellExecutor::executeRAWresizingCmd - Full Convert program : " << fullCmd;
//    convertProcess.execute(fullCmd);

//    exec_success = convertProcess.waitForFinished();

//    if(exec_success){
//        result = convertProcess.readAllStandardOutput();
//        qDebug() << "Conversion result : " << result;
//    } else {
//        errors = convertProcess.readAllStandardError();
//        qDebug() << "Conversion errors : " << errors;
//    }

//    convertProcess.close();
//    return exec_success;
//}

bool shellExecutor::executeConvertOrderedRAWsToPNG()
{
    bool exec_success;
    QProcess convertProcess;
    QString convertPath        = tPS->getConvertPath();
    QStringList conversionArgs = cS->getConvertRAW2GrayscalePNGArgs();
    qDebug() << "shellExecutor::executeConvertRAWsToPNG - What ?";

    QString errors, result;
    //convertProcess.execute(convertPath, conversionArgs);
    qDebug() << "shellExecutor::executeConvertRAWsToPNG - Conversion string "
             << convertPath << conversionArgs.join(" ");
    QString fullCmd = convertPath + " " + conversionArgs.join(" ");
    qDebug() << "shellExecutor::executeConvertRAWsToPNG - Full Convert program : " << fullCmd;
#ifdef Q_OS_WIN32
    convertProcess.execute(fullCmd);
#endif
#ifdef Q_OS_UNIX
    convertProcess.execute(convertPath, conversionArgs);
#endif
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

