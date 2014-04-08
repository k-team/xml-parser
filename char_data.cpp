#include "char_data.h"

CharData::CharData(std::string const & d):
  _data(d)
{
}

CharData::~CharData() {
}

std::string const & CharData::data() const {
  return _data;
}
