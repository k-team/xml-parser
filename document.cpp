#include "document.h"
#include "prolog.h"
#include "element.h"
#include "pi.h"

Document::Document(Prolog * p, Element * r, std::vector<PI *> const & pis):
  _prolog(p), _root(r), _endPIs(pis)
{
}

Document::~Document() {
  delete _prolog;
  delete _root;
  for (std::vector<PI *>::iterator it(_endPIs.begin());
      it != _endPIs.end(); it++) {
    delete *it;
  }
}

std::string Document::str() const
{
  std::string s = "";
  if (_prolog != nullptr)
    s += _prolog->str() + "\n";
  if (_root != nullptr)
    s += _root->str() + "\n";
  for(auto pi : _endPIs)
  {
    s += pi->str() + "\n";
  }
  return s;
}

Prolog * Document::prolog() const {
  return _prolog;
}

Element * Document::root() const {
  return _root;
}

std::vector<PI *> const & Document::endPIs() const {
  return _endPIs;
}

// vim:ft=cpp et sw=2 sts=2:
