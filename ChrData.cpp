#include "ChrData.h"

ChrData::ChrData(std::string const & d):
  _data(d)
{
}

ChrData::~ChrData() {
}

std::string const & ChrData::data() const {
  return _data;
}
