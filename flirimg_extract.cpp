#include "flirimg.h"
#include <QDebug>


bool flirImg::extractRAWs()
{
    QString unorderedRAWsResult, unorderedRAWsErrors;
    executeZeCommand(getExiftoolPath(), buildExtractionArgList(UNORDEREDRAWS),
                     unorderedRAWsResult, unorderedRAWsErrors, execute);
    return doesFileExist(unorderedRAWs_imgpath);
}

void flirImg::extractAllTags()
{
    QString tagsResult, tagsErrors;
    executeZeCommand(getExiftoolPath(), buildExtractionArgList(ALLTAGS), tagsResult, tagsErrors, start);
    qDebug() << "flirImg::extractAllTagsInOneGo -- size of result" << tagsResult.size() << " " << tagsErrors.size();
    QStringList allFlirTags = tagsResult.split('\n');
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
    imagewidth            = flirTagsMap["Exif Image Width"]                               .toInt();
    imageheight           = flirTagsMap["Exif Image Height"]                              .toInt();
    rawthermalimagewidth  = flirTagsMap["Raw Thermal Image Width"]                        .toInt();
    rawthermalimageheight = flirTagsMap["Raw Thermal Image Height"]                       .toInt();
    paletteColors         = flirTagsMap["Palette Colors"]                                 .toInt();
    // qDebug() << "flirImg::readAllTags -- Palette Colors " << paletteColors;

    planckr1              = flirTagsMap["Planck R1"]                                      .toDouble();
    planckr2              = flirTagsMap["Planck R2"]                                      .toDouble();
    planckb               = flirTagsMap["Planck B"]                                       .toDouble();
    planckf               = flirTagsMap["Planck F"]                                       .toDouble();
    plancko               = flirTagsMap["Planck O"]                                       .toDouble();
    emis                  = flirTagsMap["Emissivity"]                                     .toDouble();
    rawvaluerange         = flirTagsMap["Raw Value Range"]                                .toDouble();
    rawvaluemedian        = flirTagsMap["Raw Value Median"]                               .toDouble();
    ata1                  = flirTagsMap["Atmospheric Trans Alpha 1"]                      .toDouble();
    ata2                  = flirTagsMap["Atmospheric Trans Alpha 2"]                      .toDouble();
    atb1                  = flirTagsMap["Atmospheric Trans Beta 1"]                       .toDouble();
    atb2                  = flirTagsMap["Atmospheric Trans Beta 2"]                       .toDouble();
    atx                   = flirTagsMap["Atmospheric Trans X"]                            .toDouble();

    focusDist             = flirTagsMap["Focus Distance"]           .split(" ").at(0)     .toDouble();
    objDist               = flirTagsMap["Object Distance"]          .split(" ").at(0)     .toDouble();
    tref                  = flirTagsMap["Reflected Apparent Temperature"].split(" ").at(0).toDouble();
    atmTemp               = flirTagsMap["Atmospheric Temperature"]  .split(" ").at(0)     .toDouble();
    IRWTemp               = flirTagsMap["IR Window Temperature"]    .split(" ").at(0)     .toDouble();
    IRWTrans              = flirTagsMap["IR Window Transmission"]                         .toDouble();
    relHum                = flirTagsMap["Relative Humidity"]        .split(" ").at(0)     .toDouble();

    cmodel                = flirTagsMap["Camera Model"]                                   .toStdString();
    RawThermalImageType   = flirTagsMap["Raw Thermal Image Type"]                         .toStdString();
    palName               = flirTagsMap["Palette Name"]                                   .toStdString();

}

bool flirImg::extractPalette()
{
  // Extract Palette
  // exiftool FLIR1097.jpg -b -Palette
  // | convert -size 224X1
  //           -depth 8 YCbCr:-
  //           -separate
  //           -swap 1,2
  //           -set colorspace YCbCr
  //           -combine
  //           -colorspace RGB
  //           -auto-level
  //           Palette.png

  QString result, errors;
  executeZeCommand(getExiftoolPath(), buildExtractionArgList(PALETTE), result, errors, execute);
  if(doesFileExist(rawpalette_imgpath)){
    executeZeCommand(getConvertPath(), buildConvertArgList(RAWPALETTE_2_1DPALETTE), result, errors, start);
  }
  else{
    qDebug() << QString("flirImg::extractPalette -- File %1 could not be created").arg(rawpalette_imgpath);
    return false;
  }
}

bool flirImg::extractEmbeddedRGBImage()
{
  // Extract embedded image
  // exiftool FLIR1097.jpg -b -EmbeddedImage FLIR1097.rgb.png
    return true;
}
