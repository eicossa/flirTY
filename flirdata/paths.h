#ifndef PATHS_H
#define PATHS_H

#include <QObject>
#include <QDir>
#include "enumtypes.h"

class paths : public QObject
{
    Q_OBJECT
public:
    explicit paths(QObject *parent = nullptr);
    paths(QString);

signals:
public:
    bool static doesFileExist (const QString& fileName);
    QString static get_exiftool_path();
    QString static get_convert_path();
public:
    QStringList exiftool_convert_args(convert_cmdtype t);
    QStringList exiftool_extract_args(extract_cmdtype t);

    QString transformToWindowsPath(QString);
public:
    QString get_raws1_dpath() { return raws1_dpath; }
    QString get_raws2_dpath() { return raws2_dpath; }
    QString get_temp1_dpath() { return temp1_dpath; }
    QString get_temp2_dpath() { return temp2_dpath; }
    QString get_imgpath()     { return this->imgpath;}

    QString get_raws1_csv_dpath() { return raws1_csv_dpath; }
    QString get_raws2_csv_dpath() { return raws2_csv_dpath; }
    QString get_temp1_csv_dpath() { return temp1_csv_dpath; }
    QString get_temp2_csv_dpath() { return temp2_csv_dpath; }
public:

private:
    QString imgpath;
    QString filename;

    QString display_raws1_path, display_raws2_path;
    QString rawpalette_imgpath, palette_imgpath;
    QString temps_gray_path;

    //std::string          reorderedRAWs_imgpath;
    QString repalettedOrderedRAWs_imgpath;
    QString repalettedUnorderedRAWs_imgpath;
    QString embeddedRGB_imgpath;
    //std::string          preorderedRAWs_resized_imgpath;

    QString raws1_dpath, raws2_dpath;
    QString temp1_dpath, temp2_dpath;
    QString mdata_dpath;

    QString raws1_csv_dpath, raws2_csv_dpath;
    QString temp1_csv_dpath, temp2_csv_dpath;

    QString raws1_display_dpath, raws2_display_dpath;
    QString temp1_display_dpath, temp2_display_dpath;

private:
    // paletteColors
    int                paletteColors;

};

#endif // PATHS_H
