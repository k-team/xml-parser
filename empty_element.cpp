#include "empty_element.h"
#include "element.h"

EmptyElement::EmptyElement(std::vector<Attribute *> const & a):
  Element(a)
{
}

EmptyElement::~EmptyElement() {
}
