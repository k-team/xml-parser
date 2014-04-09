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
  for (auto a : _firstPIs)
  {
    s += a->str() + "\n";
  }
  if (_doctype != nullptr)
  {
    s += _doctype->str() + "\n";
  }
  for (auto a : _lastPIs)
  {
    s += a->str() + "\n";
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

void Prolog::to_be_or_not_to_be(std::ostream & os) const
{
  for (auto pi : _firstPIs)
  {
    pi->to_be_or_not_to_be(os);
  }
  if (_doctype != nullptr)
  {
    _doctype->to_be_or_not_to_be(os);
  }
  for (auto pi : _lastPIs)
  {
    pi->to_be_or_not_to_be(os);
  }
}

// vim:ft=cpp et sw=2 sts=2:
