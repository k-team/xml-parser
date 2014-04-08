#include "composite_element.h"
#include "content.h"
#include "attribute.h"

CompositeElement::CompositeElement(std::vector<Content *> const & c,
    std::vector<Attribute *> const & a):
  Element(a),
  _content(c)
{
}

CompositeElement::~CompositeElement()
{
  for (auto c: _content)
  {
    delete c;
  }
}

std::vector<Content *> const & CompositeElement::content() const
{
  return _content;
}

// vim:ft=cpp et sw=2 sts=2:
