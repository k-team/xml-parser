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
  for (std::vector<PI *>::iterator it = _firstPIs.begin();
      it != _firstPIs.end(); it++)
    delete *it;
  delete _doctype;
  for (std::vector<PI *>::iterator it = _lastPIs.begin();
      it != _lastPIs.end(); it++)
    delete *it;
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
