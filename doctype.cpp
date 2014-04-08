#include "doctype.h"

Doctype::Doctype(std::string const & n1,
    std::string const & n2,
    std::string const & n3):
  _n1(n1),
  _n2(n2),
  _n3(n3)
{
}

Doctype::~Doctype()
{
}

std::string Doctype::str() const
{
  return "<!DOCTYPE " + _n1 + " " + _n2 + " \"" + _n3 + "\">";
}
