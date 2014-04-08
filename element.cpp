#include "element.h"
#include "attribute.h"

Element::Element(std::vector<Attribute *> const & a):
  _attributes(a)
{
}

Element::~Element() {
  for (std::vector<Attribute *>::iterator it = _attributes.begin();
      it != _attributes.end(); it++)
    delete *it;
}

std::vector<Attribute *> const & Element::attributes() const {
  return _attributes;
}
