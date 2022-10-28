#include <QCoreApplication>
#include <QDebug>
#include <string>

#include "flirtags.h"
#include "flirtemp.h"
#include "flirmat.h"

#include "input_parser.h"
#include <tclap/CmdLine.h>

#include <QImageReader>
#include <QImageWriter>

using namespace TCLAP;
using namespace std;




void process_files1( const QString &inInputFile, const QString inOutputDir )
{
    flirtags ft(inInputFile);
    flirmat fm(&ft);
    fm.dump_mats();

}


void process_files2( const QString &inInputFile, const QString inOutputDir)
{
//   QImageReader   reader( inInputFile );
//   if ( !reader.canRead() ) {
//      qWarning() << " Could not read image" << inInputFile;
//      return;
//   }
//   QImage   image = reader.read();
//   if ( image == QImage() ) {
//      qWarning() << " Error reading image" << inInputFile;
//      return;
//   }
//   const QFileInfo   fileInfo( inInputFile );
//   const QString  cBaseFileName = fileInfo.baseName();
//   const QString  cExtension = fileInfo.suffix();

//   qInfo().noquote() << " Input image format:"
//                     << sQImageFormatToStr( image.format() );
//   // Convert QImage to a cvMat
//   cv::Mat  cvMat = ASM::QImageToCvMat( image, false );
//   qInfo().noquote() << " cv::Mat format:"
//                     << sCVTypeToStr( cvMat.type() );
//   cv::imwrite( QStringLiteral( "%1/%2_cvMat.png" )
//                .arg( inOutputDir ).arg( cBaseFileName )
//                .toLocal8Bit().constData(),
//                cvMat );
//   //cv::imwrite("./mat1.npg", cvMat);

//   sSplitAndWriteChannels( cvMat, inOutputDir, cBaseFileName );
//   // Convert back to QImage and write it out
//   QImage converted = ASM::cvMatToQImage( cvMat );
//   qInfo().noquote() << " Converted image format:"
//                     << sQImageFormatToStr( converted.format() );

//   QImageWriter   writer( QStringLiteral( "%1/%2_converted.%3" )
//                          .arg( inOutputDir ).arg( cBaseFileName )
//                          .arg( cExtension ), cExtension.toLocal8Bit() );

//   writer.write( converted );
}

void process(QString path)
{
    QDir const source(path);

//    if (!source.exists())
//        return;

//    QStringList const files = source.entryList(QStringList() << "*.jpg", QDir::Files);
//    QStringList const folders = source.entryList(QDir::NoDot | QDir::NoDotDot | QDir::Dirs);

//    QStringList const all = folders + files;

//                for (QString const& name: all)
//                {
//                    QString const fullPathName = path + QDir::separator() + name;
//                    if (QFileInfo(fullPathName).isDir())
//                    {
//                        setItems(fullPathName, level++);
//                        continue;
//                    }
//                    qDebug() << "Level " << level << " " << fullPathName;
//                }
}

void process_dir()
{
    const QString  cInputDir = TEST_INPUT_DIR;
    //   const QString  cOutputDir( "./output" );
    const QString cOutputDir = TEST_OUTPUT_DIR;

    qInfo() << "  ip dir:" << cInputDir;
    qInfo() << "  op dir:" << cOutputDir;

    QDir  outputDir( cOutputDir );
    outputDir.removeRecursively();
    QDir().mkpath( cOutputDir );

    QStringList nameFilter;
    nameFilter << "*.png" << "*.jpg";

    const QFileInfoList  cInfo = QDir( cInputDir ).entryInfoList(nameFilter,
                                                                 QDir::Files |
                                                                 QDir::Readable |
                                                                 QDir::NoDotAndDotDot |
                                                                 QDir::NoSymLinks );




    for ( const QFileInfo &fileInfo : cInfo )
    {
       qInfo() << "====";
       qInfo() << "Processing:" << fileInfo.fileName();

       process_files1( fileInfo.filePath(), cOutputDir );
       // sRunTests( fileInfo.filePath(), cOutputDir );
    }
}



void process_asm(int argc, char** argv)
{
    QCoreApplication a(argc, argv);

    try {
        // commandline object
        TCLAP::CmdLine cmd("flirdata v0.1 Author: Arjun J Rao ajrao@barc.gov.in", ' ', "0.9");

        // filename
        TCLAP::ValueArg<std::string> fname_arg("f","filename", "filename of thermal image",
                                               true,"flirimg.tiff","string");
        cmd.add(fname_arg);

        // dirname
        TCLAP::ValueArg<std::string> dname_arg("d","directory","directory of thermal images",
                                               true,"flirimg.tiff","string");
        cmd.add(dname_arg);

        // csv_switch
//        TCLAP::SwitchArg csv_switch("c","csv","dump files as csv", cmd, false);
//        cmd.add(csv_switch);

        // json_switch
        TCLAP::SwitchArg json_switch("j", "json", "dump files as json", cmd, false);
        cmd.add(json_switch);

        // png_switch
        TCLAP::SwitchArg png_switch("p", "png", "dump files as png", cmd, false);
        cmd.add(png_switch);

        // tiff_switch
        TCLAP::SwitchArg tiff_switch("t", "tiff", "dump files as tiff", cmd, false);
        cmd.add(tiff_switch);

        // binary mat switch
        TCLAP::SwitchArg binary_switch("b", "opencv binary Mat", "dump files as binary Mat", cmd, true);
        cmd.add(binary_switch);

        // cmd.add(fname_arg);
        // cmd.add(dname_arg);
        cmd.parse( argc, argv );

        // Get the value parsed by each arg.
        std::string fname = fname_arg.getValue();
        flirtags ftags(QString::fromStdString(fname));
        flirmat fm(&ftags);

        std::string dname = dname_arg.getValue();

        // Do what you intend.
//        if(csv_switch) {

//        }
//        else {

//        }
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    // QString ipath="/home/eicossa/Data/nabtd-images/old/FLIR1097/FLIR1097.jpg";
    // flirdata f(ipath);
    // flirtemp ft(&f);

    // f.read_metadata(ipath);
    // return a.exec();
}


int main(int argc, char *argv[])
{
    QCoreApplication   app( argc, argv );
    process_dir();
}
