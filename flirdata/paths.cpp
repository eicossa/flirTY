#include "paths.h"
#include "utilfuncs.h"
using namespace envt;

paths::paths(QObject *parent) : QObject(parent)
{

}

paths::paths(QString p)
{
    imgpath = p;

    filename                        = imgpath.split('.').at(0);

//    QStringLiteral( "%1.%2.%3" )
//                    .arg( inOutputDir ).arg( cBaseFileName ).arg( cExtension )
//                    .toLocal8Bit().constData()

    raws1_dpath                     = QString(filename + ".raws1.pgm");
    raws2_dpath                     = QString(filename + ".raws2.pgm");
    temp1_dpath                    = QString(filename + ".temps1.dat");
    temp2_dpath                    = QString(filename + ".temps2.dat");
    mdata_dpath                     = QString(filename + ".mdata.txt");

    raws1_csv_dpath                 = QString(filename + ".raws1.csv");
    raws2_csv_dpath                 = QString(filename + ".raws2.csv");
    temp1_csv_dpath                = QString(filename + ".temps1.csv");
    temp2_csv_dpath                = QString(filename + ".temps2.csv");

    raws1_display_dpath             = QString(filename + ".raws1.display.png");
    raws2_display_dpath             = QString(filename + ".raws2.display.png");
    temp1_display_dpath            = QString(filename + ".temps1.display.png");
    temp2_display_dpath            = QString(filename + ".temps2.display.png");


    // embeddedRGB_imgpath             = QString(filename + ".embeddedrgb.png");



    // rawpalette_imgpath              = QString(filename + ".rawpalette.png");
    // palette_dpath                 = QString(filename + ".pal.png");

    // metadata_fpath                  = QString(filename + ".metadata.txt");
}



QStringList paths::exiftool_extract_args( extract_cmdtype t )
{
    QStringList argList;
    switch(t)
    {
        case ALLTAGS:
            argList = QStringList() << "-a"
                                    << imgpath;
            break;
        case RAWS1:
            argList = QStringList() << imgpath
                                    << "-RawThermalImage" << "-b"
                                    << "-W" << raws1_dpath;
            break;
        case PALETTE:
            argList = QStringList() << imgpath
                                    << "-b -Palette"
                                    << "-W" << rawpalette_imgpath;
    }
    return argList;
}

QStringList paths::exiftool_convert_args(convert_cmdtype t)
{
    QStringList argList;
    switch(t)
    {
        case RAWS2_DISPLAY :
            envt::transformToWindowsPath(raws2_dpath);
            //QString sizeStr1 = QString::number(rawthermalimagewidth)+"X"+QString::number(rawthermalimageheight);
            //QString sizeStr1 = QString::number(rawthermalimageheight);
            argList = QStringList()
                      << raws2_dpath
                      //<< "-size"
                      //<< "640x480"
                      << raws2_display_dpath;
            break;
        case RAWS1_DISPLAY :
            envt::transformToWindowsPath(raws2_dpath);
            argList = QStringList()
                      << raws2_dpath
                      << raws1_display_dpath;
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


