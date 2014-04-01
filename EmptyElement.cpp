#include "EmptyElement.h"
#include "Element.h"

EmptyElement::EmptyElement(std::vector<Attribute *> const & a):
  Element(a)
{
}

EmptyElement::~EmptyElement() {
}
