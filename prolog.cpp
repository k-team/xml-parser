#include "prolog.h"
#include "xml_decl.h"
#include "pi.h"
#include "doctype.h"

Prolog::Prolog(XMLDecl * d,
    std::vector<PI *> const & firsts,
    Doctype * t,
    std::vector<PI *> const & lasts):
  _xmlDecl(d),
  _firstPIs(firsts),
  _doctype(t),
  _lastPIs(lasts)
{
}

Prolog::~Prolog() {
  delete _xmlDecl;
  for (std::vector<PI *>::iterator it = _firstPIs.begin();
      it != _firstPIs.end(); it++)
    delete *it;
  delete _doctype;
  for (std::vector<PI *>::iterator it = _lastPIs.begin();
      it != _lastPIs.end(); it++)
    delete *it;
}

XMLDecl * xmlDecl() const {
  return _xmlDecl;
}

std::vector<PI *> const & firstPIs() const {
  return _firstPIs;
}
Doctype * doctype() const {
  return _doctype;
}

std::vector<PI *> const & lastPIs() const {
  return _lastPIs;
}
