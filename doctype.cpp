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
  std::string s("<!DOCTYPE");
  if (!_n1.empty())
    s += " " + _n1;
  if (!_n2.empty())
    s += " " + _n2;
  if (!_n3.empty())
    s += " \"" + _n3 + "\"";
  return s + ">";
}
