#include "element.h"
#include "attribute.h"

Element::Element(std::vector<Attribute *> const & a):
  Base(), _attributes(a)
{
}

Element::~Element()
{
  for (auto attr: _attributes)
  {
    delete attr;
  }
}

std::vector<Attribute *> const & Element::attributes() const {
  return _attributes;
}

// vim:ft=cpp et sw=2 sts=2:
