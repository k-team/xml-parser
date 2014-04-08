#include "empty_element.h"
#include "element.h"

EmptyElement::EmptyElement(std::vector<Attribute *> const & a):
  Element(a)
{
}

EmptyElement::~EmptyElement() {
}

// vim:ft=cpp et sw=2 sts=2:
