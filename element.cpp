#include "element.h"
#include "attribute.h"
#include "xml_exception.h"
#include <set>
#include <sstream>

Element::Element(std::string const & name, Element::attribute_list const & attrs):
  _name(name), _attributes(attrs)
{
}

Element::~Element()
{
  for (auto attr : _attributes)
  {
    delete attr;
  }
}

std::string const & Element::name() const
{
  return _name;
}

Element::attribute_list const & Element::attributes() const
{
  return _attributes;
}

std::string Element::str() const
{
  std::ostringstream oss;
  oss << "<" << _name << " ";
  for (auto attr : _attributes)
  {
    oss << attr->str() << " ";
  }
  oss << "/>";
  return oss.str();
}

void Element::to_be_or_not_to_be() const
{
  std::set<std::string> attr_names;
  for (auto attr : _attributes)
  {
    attr->to_be_or_not_to_be();
    if (!attr_names.insert(attr->name()).second)
    {
      throw XmlException("Attribute names must be unique",
          "duplicate name " + attr->name());
    }
  }
}

// vim:ft=cpp et sw=2 sts=2:
