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

std::string CharData::str() const
{
  return _data;
}

// vim:ft=cpp et sw=2 sts=2:
