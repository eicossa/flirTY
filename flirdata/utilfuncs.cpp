#include <QDebug>
#include <QProcess>
#include "utilfuncs.h"
#include "errmsgs2.h"
#include <sys/stat.h>
#include <unistd.h>

namespace utilfuncs {
    bool doesFileExist (const QString& fileName) {
      struct stat buffer;
      return (stat (fileName.toStdString().c_str(), &buffer) == 0);
    }

    // read_file_to_mat
    bool read_file_to_mat(QString aPath, cv::Mat &aMat, cv::ImreadModes flags)
    {
        aMat = imread(aPath.toStdString(), flags);

        if(!aMat.empty()) {
          return true;
        } else {
            qDebug() << "read_file_to_mat "
                     //<< QString::fromStdString(emsgs->getThermalImgErrorMsg())
                     << " | "
                     << aPath;
            return false;
        }
    }

    // execute_cmd
    void execute_cmd(QString cmdPath, QStringList cmdArgs,
                     QString &result, QString &errors,
                     cmdExecType t)
    {
        bool    exec_success;
        QProcess cmdproc;

        QString fullCmd = cmdPath + " " + cmdArgs.join(" ");
        qDebug() << "utilfuncs::execute_cmd : " << fullCmd;
    #ifdef Q_OS_WIN32
        //qDebug()<<"flirImg::executeZeCommand -- works here";
        cmdProcess.start(fullCmd);
    #endif
    #ifdef Q_OS_UNIX
        if(t == execute)
            cmdproc.execute(cmdPath, cmdArgs);
        else if(t == start)
            cmdproc.start(cmdPath, cmdArgs);
    #endif
        exec_success = cmdproc.waitForFinished();

        result = cmdproc.readAllStandardOutput();
        errors = cmdproc.readAllStandardError();
        if(exec_success){
            //qDebug() << "flirImg::executeZeCommand - Size of result is " << result.size();
            //qDebug() << "flirImg::executeZeCommand - Conversion result : " << result;
        } else{
            //qDebug() << "flirImg::executeZeCommand - Size of errors is " << errors.size();
            //qDebug() << "flirImg::executeZeCommand - Conversion errors : " << errors;
        }

        qDebug() << "utilfuncs::execute_cmd : Done";
        cmdproc.close();
    }



    void display_opencv(std::string window_name, Mat m)
    {
        // child process
        namedWindow(window_name, WINDOW_NORMAL);
        setWindowProperty(window_name, WND_PROP_AUTOSIZE, WINDOW_AUTOSIZE);
        imshow(window_name, m);
    }
}


namespace envt {
QString get_exiftool_path()
{
    QString exiftoolPath;
    #ifdef Q_OS_WIN32
    exiftoolPath = QDir::currentPath() + "/nabtdimgs/exiftool";
    #elseif Q_OS_MACX
    exiftoolPath = "/usr/local/bin/exiftool";
    #else Q_OS_LINUX
    exiftoolPath = "/usr/bin/exiftool";
    #endif
    return exiftoolPath;
}

QString get_convert_path()
{
    QString convertPath;
    #ifdef Q_OS_WIN32
    convertPath = QDir::currentPath() + "/nabtdimgs/convert";
    convertPath = transformToWindowsPath(convertPath);
    // convertPath = "C:/opencvqt5/flirbaba-gui/imagemagick/convert does not work
    // gives an error like Unrecognized option -depth 16
    #elif Q_OS_MACX
    convertPath  = "/usr/local/bin/convert";
    #else Q_OS_LINUX
    convertPath  = "/usr/local/bin/convert";
    #endif
    return convertPath;
}

QString transformToWindowsPath(QString str)
{
    QString transformedPath;
#ifdef Q_OS_WIN32
    std::replace(str.begin(), str.end(),
                 QChar('/'), QChar('\\'));
#endif
    return str;
}
}



namespace writers {
void write_mat_to_csv(QString filename, Mat m)
{
   ofstream myfile;
   myfile.open(filename.toStdString().c_str());
   myfile<< cv::format(m, cv::Formatter::FMT_CSV) << std::endl;
   myfile.close();
}

void write_mat(QString filename, Mat m)
{
    // ofstream myfile;
    // myfile.open(filename.toStdString().c_str());
    // myfile<< cv::format(m, cv::Formatter::FMT_) << std::endl;
    cv::imwrite(filename.toStdString(), m);
    // myfile.close();
}

void write_mat_to_json(string filename, Mat m)
{

}

void write_mat_to_png(string filename, Mat m)
{

}

void write_mat_to_tiff(string filename, Mat m)
{

}
}
