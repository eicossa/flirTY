#include "flirtemp2.h"
#include <QDebug>



flirtemp2::flirtemp2(flirtags* ftags, const cv::Mat raws_mat) :
    raws_m(raws_mat),
    ft(ftags),
    w(ftags->imagewidth),
    h(ftags->imageheight)
{
    pixels2=std::vector<std::vector<pixData>>(h);

    for(int rowNum=0; rowNum < h; rowNum++){
        pixels2[rowNum] = std::vector<pixData>(w);
        for(int colNum=0; colNum < w; colNum++){
            pixData p;
            p.type = formulaType::formula3;
            init3(p.f3);
            // p.f3 = {-1,-1,-1,-1,-1,-1,-1,-1};
            pixels2[rowNum][colNum] = p;
        }
    }

    // cv::Mat()
    isReordered = false;
    if(ft->RawThermalImageType == "PNG") {
        isReordered = true;
    }
    calc_temp();

    ft->rawMinObj = calc_rawobjv(ft->rawMin);
    ft->rawMaxObj = calc_rawobjv(ft->rawMax);
    ft->tmin = calc_temp_from_rawobjv(ft->rawMinObj);
    ft->tmax = calc_temp_from_rawobjv(ft->rawMaxObj);

}


int flirtemp2::get_raw_at(int rowNum, int colNum)
{
    return pixels2[rowNum][colNum].f3.raw;
}

double flirtemp2::get_temp_at(int rowNum, int colNum)
{
    qDebug() << "flirtemp2::get_temp_at() - " << QString::fromStdString(pixels2[rowNum][colNum].f3.debugstring());
    return pixels2[rowNum][colNum].f3.t1;
}

void flirtemp2::calc_temp()
{
  for(int rowNum=0; rowNum < h; rowNum++){
      for(int colNum=0; colNum < w; colNum++){
          pixData p;
          //        if(!isReordered) {
          //            p.type  = formulaType::formula1;
          //            formula1(rowNum, colNum, p.f1);
          //        }
          //        else {
          //            p.type = formulaType::formula2;
          //            formula2(rowNum, colNum, p.f2);
          //        }

          // formula3 is for debugging
          p.type = formulaType::formula3;
          formula3(rowNum, colNum, p.f3);
          pixels2[rowNum][colNum] = p;
      }
  }
}

void flirtemp2::debug_temps()
{
    for(int i=0;i<5;i++) {
        int rr = random()%h;
        int rc = random()%w;
        qDebug() << QString::fromStdString(pixels2[rr][rc].f3.debugstring());
    }
    //ft->img_summ5();
}

void flirtemp2::formula1(int rowNum, int colNum, f1data& f)
{
    f.raw = raws_m.at<unsigned short>(rowNum, colNum);
    f.rawobjv = calc_rawobjv(f.raw);
    f.t = calc_temp_from_rawobjv(f.rawobjv);

    f.rowNum=rowNum;
    f.colNum=colNum;
}

void flirtemp2::formula2(int rowNum, int colNum, f2data& f)
{
    f.raw = raws_m.at<unsigned short>(rowNum, colNum);
    f.raw2 = change_byteorder(f.raw);
    f.rawobjv = calc_rawobjv(f.raw2);
    f.t = calc_temp_from_rawobjv(f.rawobjv);

    f.rowNum=rowNum;
    f.colNum=colNum;
}

void flirtemp2::formula3(int rowNum, int colNum, f3data& f)
{
    f.raw = raws_m.at<unsigned short>(rowNum, colNum);
    f.rawobjv1 = calc_rawobjv(f.raw);
    f.t1 = calc_temp_from_rawobjv(f.rawobjv1);

    f.raw2 = change_byteorder(f.raw);
    f.rawobjv2 = calc_rawobjv(f.raw2);
    f.t2 = calc_temp_from_rawobjv(f.rawobjv2);

    f.rowNum=rowNum;
    f.colNum=colNum;
}

void flirtemp2::init3(f3data& f)
{
    f.raw = -1;
    f.rawobjv1 = -1;
    f.t1 = -1;

    f.raw2 = -1;
    f.rawobjv2 = -1;
    f.t2 = -1;

    f.rowNum = -1;
    f.colNum = -1;
}

int flirtemp2::change_byteorder(unsigned short wrong_byte_order)
{
    unsigned short x, y, x1, x2, x3;
    unsigned short reordered_val;
    x               = wrong_byte_order;
    x1              = x >> 8;
    x2              = (x & 0x0ff);
    x3              = x2 << 8;
    y               = x1 + x3;
    reordered_val = y;
    return reordered_val;
}


#include <cmath>

double flirtemp2::calc_rawobjv(int r)
{
    // double E        = 1; double OD       = 1;
    // double RTemp    = 20;
    // double ATemp    = RTemp;
    // double IRWTemp  = RTemp;
    // double IRT      = 1;
    // double RH       = 50;
    // double planckr1 = 21106.77;
    // double planckb  = 1501;
    // double planckf  = 1;
    // double plancko  = -7340;
    // double planckr2 = 0.012545258;
    // double ATA1     = 0.006569;
    // double ATA2     = 0.01262;
    // double ATB1     = -0.002276;
    // double ATB2     = -0.00667;
    // double ATX      = 1.9;
    // emiss.wind      = 1 - IRT;
    // refl.wind       = 0 # anti-reflective coating on window

    int    raw      = r;
    double E        = ft->emis;
    double OD       = ft->objDist;
    double RTemp    = ft->tref;
    double ATemp    = ft->atmTemp;
    double IRWTemp  = ft->IRWTemp;
    double IRT      = ft->IRWTrans;
    double RH       = ft->relHum;
    double planckr1 = ft->planckr1;
    double planckb  = ft->planckb;
    double planckf  = ft->planckf;
    double plancko  = ft->plancko;
    double planckr2 = ft->planckr2;
    double ATA1     = ft->ata1;
    double ATA2     = ft->ata2;
    double ATB1     = ft->atb1;
    double ATB2     = ft->atb2;
    double ATX      = ft->atx;
    double emiss_wind = 1 - IRT;
    double refl_wind  = 0;


    double h2o = (RH/100)*exp(1.5587
                              + 0.06939 * ATemp
                              - 0.00027816 * pow(ATemp, 2)
                              + 0.00000068455 * pow(ATemp, 3));
    double tau1 = ATX * exp(-sqrt(OD/2) * (ATA1 + ATB1 * sqrt(h2o)))
                  + (1 - ATX) * exp(-sqrt(OD/2) * (ATA2 + ATB2 * sqrt(h2o)));
    double tau2 = ATX * exp(-sqrt(OD/2) * (ATA1 + ATB1 * sqrt(h2o)))
                  + (1-ATX) * exp(-sqrt(OD/2) * (ATA2 + ATB2 * sqrt(h2o)));
    double raw_refl1 = planckr1/(planckr2*(exp(planckb/(RTemp+273.15))-planckf)) - plancko;
    double raw_refl1_attn = (1-E)/E*raw_refl1;
    double raw_atm1 = planckr1/(planckr2*(exp(planckb/(ATemp+273.15))-planckf)) - plancko;
    double raw_atm1_attn = (1-tau1)/E/tau1 * raw_atm1;
    double raw_wind = planckr1/(planckr2*(exp(planckb/(IRWTemp+273.15))-planckf)) - plancko;
    double raw_wind_attn = emiss_wind/E/tau1/IRT * raw_wind;
    double raw_refl2 = planckr1/(planckr2*(exp(planckb/(RTemp+273.15))-planckf)) - plancko;
    double raw_refl2_attn = refl_wind/E/tau1/IRT * raw_refl2;
    double raw_atm2 = planckr1/(planckr2*(exp(planckb/(ATemp+273.15))-planckf)) - plancko;
    double raw_atm2_attn = (1-tau2)/E/tau1/IRT/tau2 * raw_atm2;
    double raw_obj = (raw/E/tau1/IRT/tau2
                      - raw_atm1_attn
                      - raw_atm2_attn
                      - raw_wind_attn
                      - raw_refl1_attn
                      - raw_refl2_attn);
//    if(raw == 14128 && printCounter==0){
//        printCounter+=1;
//        qDebug() << " RTemp " << tref << " ATemp " << ATemp << " IRWTemp " << IRWTemp;
//        qDebug() << " For raw " << raw << " raw_obj = " << raw_obj;

//        qDebug() << " raw_refl1 " << raw_refl1 << " raw_refl1_attn " << raw_refl1_attn;
//        qDebug() << " raw_atm1  "  << raw_atm1  << " raw_atm1_attn "  << raw_atm1_attn;
//        qDebug() << " raw_wind  "  << raw_wind  << " raw_wind_attn "  << raw_wind_attn;
//        qDebug() << " raw_refl2 " << raw_refl2 << " raw_refl2_attn " << raw_refl2_attn;
//        qDebug() << " raw_atm2  "  << raw_atm2  << " raw_atm2_attn "  << raw_atm2_attn;

//    }

    return raw_obj;
}

double flirtemp2::calc_temp_from_rawobjv(double rawobjvalue)
{
    double planckb  = ft->planckb;
    double planckr1 = ft->planckr1;
    double planckr2 = ft->planckr2;
    double plancko  = ft->plancko;
    double planckf  = ft->planckf;
    double tempC = planckb/log(planckr1/(planckr2*(rawobjvalue + plancko))+planckf)-273.15;
    return tempC;
}
