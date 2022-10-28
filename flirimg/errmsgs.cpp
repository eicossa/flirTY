#include "errmsgs.h"

ErrMsgs::ErrMsgs()
{

}

std::string ErrMsgs::getRGBimgErrorMsg()
{
  std::string errmsg = "problem loading original FLIR img";
  return errmsg;
}

std::string ErrMsgs::getThermalImgErrorMsg()
{
  std::string errmsg = "problem loading thermal data from img";
  return errmsg;
}

std::string ErrMsgs::getPHPimgErrorMsg()
{
  std::string errmsg = "problem loading data from PHP img";
  return errmsg;
}
