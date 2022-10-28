#ifndef UTILFUNCS_H
#define UTILFUNCS_H

#include "enumtypes.h"
#include <QString>
#include "opencv2/opencv.hpp"
using namespace cv;

namespace utilfuncs {
    bool doesFileExist (const QString& fileName);
    bool read_file_to_mat(QString aPath, cv::Mat &aMat, cv::ImreadModes flags);

    void execute_cmd(QString cmdPath, QStringList cmdArgs, QString &result, QString &errors, cmdExecType t);
}

namespace writers {
    void write_mat(QString filename, Mat m);
    void write_mat_to_csv(QString filename, Mat m);
}


namespace envt {
    QString get_exiftool_path();
    QString get_convert_path();
    QString transformToWindowsPath(QString str);
}
#endif // UTILFUNCS_H
