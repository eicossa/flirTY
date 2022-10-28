#include "errmsgs2.h"

errmsgs2::errmsgs2(QObject *parent) : QObject(parent)
{

}

std::string errmsgs2::getRGBimgErrorMsg()
{
  std::string errmsg = "problem loading original FLIR img";
  return errmsg;
}

std::string errmsgs2::getThermalImgErrorMsg()
{
  std::string errmsg = "problem loading thermal data from img";
  return errmsg;
}

std::string errmsgs2::getPHPimgErrorMsg()
{
  std::string errmsg = "problem loading data from PHP img";
  return errmsg;
}
