#include "composite_element.h"
#include "content.h"
#include "attribute.h"
#include <sstream>

CompositeElement::CompositeElement(std::string const & name,
    std::string const & end,
    Element::attribute_list const & attrs,
    CompositeElement::content_list const & content):
  Element(name, attrs),
  _end(end),
  _content(content)
{
}

CompositeElement::~CompositeElement()
{
  for (auto c: _content)
  {
    delete c;
  }
}

CompositeElement::content_list const & CompositeElement::content() const
{
  return _content;
}

std::string CompositeElement::str() const
{
  std::ostringstream oss;
  oss << "<" << name() << " ";
  for (auto attr: attributes())
  {
    oss << attr->str() << " ";
  }
  oss << ">";
  for (auto c: _content)
  {
    oss << c->str();
  }
  oss << "</" << name() << ">";
  return oss.str();
}

// vim:ft=cpp et sw=2 sts=2:
