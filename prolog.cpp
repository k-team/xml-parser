#include "prolog.h"
#include "xml_decl.h"
#include "pi.h"
#include "doctype.h"

Prolog::Prolog(std::vector<PI *> const & firsts, Doctype * t,
    std::vector<PI *> const & lasts):
  Base(),
  _firstPIs(firsts),
  _doctype(t), _lastPIs(lasts)
{
}

Prolog::~Prolog() {
  for (auto a : _firstPIs)
    delete a;
  delete _doctype;
  for (auto a : _lastPIs)
    delete a;
}

std::string Prolog::str() const
{
  std::string s = "";
  for (auto a : _firstPIs) {
    s += a->str() + "\n";
  }
  s += _doctype->str() + "\n";
  for (auto a : _lastPIs) {
    s += a->str() + "\n";
  }
  return s;
}

std::vector<PI *> const & Prolog::firstPIs() const {
  return _firstPIs;
}

Doctype * Prolog::doctype() const {
  return _doctype;
}

std::vector<PI *> const & Prolog::lastPIs() const {
  return _lastPIs;
}

// vim:ft=cpp et sw=2 sts=2:
