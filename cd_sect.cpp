#include "cd_sect.h"

CDSect::CDSect(std::string const & s):
  _sect(s)
{
}

CDSect::~CDSect() {
}

std::string const & CDSect::sect() const {
  return _sect;
}
