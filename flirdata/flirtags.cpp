#include "flirtags.h"
#include "paths.h"
#include "utilfuncs.h"
using namespace utilfuncs;
#include <QDebug>
#include <QProcess>

flirtags::flirtags(QString imgpath)
{
    p = new paths(imgpath);
    read_metadata();
    calc_metadata();
    print_img_summary();
    print_img_summary2();
    img_summ5();
}

void flirtags::read_metadata()
{
    extract_tags(envt::get_exiftool_path());
    // qDebug() << "... extract_tags done.";
    calc_rmin_rmax();
    // print_img_summary();
}

void flirtags::calc_metadata()
{
    rawMax = rawvaluemedian + (rawvaluerange/2);
    rawMin = rawMax - rawvaluerange;
    rawRefl = 0; // nothing is done to calculate it even in the original
}

void flirtags::extract_tags(QString exiftool_path)
{
    QString tagsResult, tagsErrors;
    utilfuncs::execute_cmd(exiftool_path, p->exiftool_extract_args(ALLTAGS),
                     tagsResult, tagsErrors, start);
    qDebug() << "flirdata::extract_tags -- size of result" << tagsResult.size() << " " << tagsErrors.size();
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

    filename              = flirTagsMap["File Name"];
}

void flirtags::calc_rmin_rmax()
{
    rawMax = rawvaluemedian + (rawvaluerange/2);
    rawMin = rawMax - rawvaluerange;

//    tmin = calcTempForOnePixel(rawMin);
//    tmax = calcTempForOnePixel(rawMax);
//    qDebug() << "Tmin : " << tmin << " | " << "Tmax : " << tmax;
}

void flirtags::img_summ5()
{
    std::stringstream p1,p2,pb,pf,po,trefs,emiss,rvmedians,rvranges;
    p1    << setw(12) << "PlanckR1 : " << std::left << setw(6) << planckr1 << "";
    p2    << setw(12) << "PlanckR2 : " << std::left << setw(6) << planckr2 << "";
    pb    << setw(12) << "PlanckB : " << std::left << setw(6) << planckb  << "";
    pf    << setw(12) << "PlanckF : " << std::left << setw(6) << planckf  << "";
    po    << setw(12) << "PlanckO : " << std::left << setw(6) << plancko  << "";
    trefs << setw(6) << "Tref : " << std::left << setw(6) << tref << "";
    emiss << setw(6) << "Emis : " << std::left << setw(6) << emis << "";
    rvmedians << setw(18) << "RawValueMedian : " << std::left << setw(6) << rawvaluemedian << "";
    rvranges  << setw(18) << "RawValueRange  : " << std::left << setw(6) << rawvaluerange << "";

    std::stringstream readmetadatas, readmetadatas2;
    readmetadatas << p1.str()
                  << p2.str()
                  << pb.str()
                  << pf.str()
                  << po.str()
                  << trefs.str()
                  << emiss.str()
                  << rvmedians.str()
                  << rvranges.str() << endl;
    cout << readmetadatas.str();
}

void flirtags::print_img_summary()
{
  //cout << "\n\n\nExiftags read .... "            << endl;
  std::stringstream orig;

  std::stringstream rmaxs,rmins,rrefls,rmaxos,rminos;
  std::stringstream tmaxs,tmins;


  std::stringstream readmetadatas, calcmetadatas1, calcmetadatas2, calcmetadatas;



  tmins << " Tmin : "        << setw(6) << tmin;
  tmaxs << " Tmax : "        << setw(6) << tmax;
  rmaxs << " RAWmax : " << setw(6) << rawMax;
  rmins << " RAWmin : " << setw(6) << rawMin;

  rmaxos << " RAWmaxobj : " << setw(6) << rawMaxObj;
  rminos << " RAWminobj : " << setw(6) << rawMinObj;
  rrefls << " RAWrefl   : " << setw(6) << rawRefl;

//  orig << " |-----------------------------|"
//       << endl
//       << p1.str()
//       << rmaxs.str()
//       << tmaxs.str()
//       << endl
//       << p2.str()
//       << rmins.str()
//       << tmins.str()
//       << endl
//       << pb.str()
//       << rrefls.str()
//       << endl
//       << pf.str()
//       << rmaxos.str()
//       << endl
//       << po.str()
//       << rminos.str()
//       << endl
//       << trefs.str()
//       << endl
//       << emiss.str()
//       << endl
//       << rvmedians.str()
//       << endl
//       << rvranges.str()
//       << endl
//       << " |-----------------------------|"
//       << endl;

  calcmetadatas1 << "|"  << rmins.str()
                 << "|" << rminos.str()
                 << "|" << tmins.str() << " |";
  calcmetadatas2 << "|"   << rmaxs.str()
                 << "|" << rmaxos.str()
                 << "|" << tmaxs.str() << " |";
  calcmetadatas << calcmetadatas1.str() << endl << calcmetadatas2.str() << endl;
  //cout << orig.str();
  cout << calcmetadatas.str();
}

void flirtags::print_img_summary2()
{
    stringstream dims,dims1,dims2;
    stringstream widths, heights, twidths, theights, horline;
    widths   << " | Width           : " << setw(10) << imagewidth << endl;
    heights  << " | Height          : " << setw(10) << imageheight << endl;
    twidths  << " | ThermalWidth    : " << setw(10) << rawthermalimagewidth  << endl;
    theights << " | ThermalHeight   : " << setw(10) << rawthermalimageheight << endl;
    horline  << " | ------------------";

    dims1  << "[" << imagewidth << "x" << imageheight << "]";
    dims2  << "(" << rawthermalimagewidth << "x" << rawthermalimageheight << ")";

    dims << dims1.str() << dims2.str();
    cout << dims.str() << endl;
}
