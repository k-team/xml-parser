#include "Document.h"
#include "Prolog.h"
#include "Element.h"
#include "PI.h"

Document::Document(Prolog * p, Element * r, PI * pi):
  _prolog(p), _root(r), _endPI(pi)
{
}

Document::~Document() {
  delete _prolog;
  delete _root;
  delete _endPI;
}

Prolog * Document::prolog() const {
  return _prolog;
}

Element * Document::root() const {
  return _root;
}

PI * Document::endPI() const{
  return _endPI;
}
