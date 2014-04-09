#include "prolog.h"
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
  {
    delete a;
  }
  for (auto a : _lastPIs)
  {
    delete a;
  }
  delete _doctype;
}

std::string Prolog::str() const
{
  std::string s;
  for (auto pi : _firstPIs)
  {
    s += pi->str();
  }
  if (_doctype != nullptr)
  {
    s += _doctype->str();
  }
  for (auto pi : _lastPIs)
  {
    s += pi->str();
  }
  return s;
}

std::vector<PI *> const & Prolog::firstPIs() const
{
  return _firstPIs;
}

Doctype * Prolog::doctype() const
{
  return _doctype;
}

std::vector<PI *> const & Prolog::lastPIs() const
{
  return _lastPIs;
}

void Prolog::to_be_or_not_to_be() const
{
  for (auto pi : _firstPIs)
  {
    pi->to_be_or_not_to_be();
  }
  if (_doctype != nullptr)
  {
    _doctype->to_be_or_not_to_be();
  }
  for (auto pi : _lastPIs)
  {
    pi->to_be_or_not_to_be();
  }
}

// vim:ft=cpp et sw=2 sts=2:
