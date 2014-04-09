#include "composite_element.h"
#include "content.h"
#include "attribute.h"
#include "xml_exception.h"
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
  oss << "<" << begin_tag();
  for (auto attr: attributes())
  {
    oss << " " << attr->str();
  }
  oss << ">\n";
  for (auto c: _content)
  {
    std::string s = c->str();
    size_t from = 0;
    size_t pos;
    while ((pos = s.find("\n", from)) != std::string::npos)
    {
      s.replace(pos, 1, "\n  ");
      from = pos + 3;
    }
    oss << "  " << s << "\n";
  }
  oss << "</" << end_tag() << ">";
  return oss.str();
}

std::string const & CompositeElement::begin_tag() const
{
  return name();
}

std::string const & CompositeElement::end_tag() const
{
  return _end;
}

void CompositeElement::to_be_or_not_to_be() const
{
  Element::to_be_or_not_to_be();
  if (begin_tag() != end_tag())
  {
    throw XmlException("Non matching element names",
        begin_tag() + " and " + end_tag());
  }
}

// vim:ft=cpp et sw=2 sts=2:
