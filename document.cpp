#include "document.h"
#include "prolog.h"
#include "element.h"
#include "pi.h"

Document::Document(Prolog * p, Element * r, std::vector<PI *> const & pis):
  _prolog(p), _root(r), _endPIs(pis)
{
}

Document::~Document()
{
  delete _prolog;
  delete _root;
  for (auto pi : _endPIs)
  {
    delete pi;
  }
}

std::string Document::str() const
{
  std::string s;
  if (_prolog != nullptr)
  {
    s += _prolog->str();
  }
  if (_root != nullptr)
  {
    s += _root->str();
  }
  for (auto pi : _endPIs)
  {
    s += pi->str();
  }
  return s;
}

Prolog * Document::prolog() const
{
  return _prolog;
}

Element * Document::root() const
{
  return _root;
}

std::vector<PI *> const & Document::endPIs() const
{
  return _endPIs;
}

void Document::to_be_or_not_to_be() const
{
  if (_root != nullptr)
  {
    _root->to_be_or_not_to_be();
  }
  else
  {
    // TODO error here ?
  }
  if (_prolog != nullptr)
  {
    _prolog->to_be_or_not_to_be();
  }
  for (auto pi : _endPIs)
  {
    pi->to_be_or_not_to_be();
  }
}

// vim:ft=cpp et sw=2 sts=2:
