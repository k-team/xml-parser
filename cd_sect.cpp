#include "cd_sect.h"

CDSect::CDSect(std::string const & s):
  _sect(s)
{
}

CDSect::~CDSect()
{
}

std::string CDSect::str() const
{
  return "<![CDATA[" + _sect + "]]>";
}

std::string const & CDSect::sect() const
{
  return _sect;
}

// vim:ft=cpp et sw=2 sts=2:
