#include "element.h"
#include "attribute.h"
#include "helpers.h"
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

Attribute * Element::find_attribute(std::string const & name) const
{
  for (auto attr : _attributes)
  {
    if (attr->name() == name)
    {
      return attr;
    }
  }
  return nullptr;
}

std::pair<std::string, std::string> Element::ns_split() const
{
  auto split_name = Helpers::split(_name, ':');
  if (split_name.size() < 2)
  {
    return { "", split_name[0] };
  }
  return { split_name[0], split_name[1] };
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

void Element::to_be_or_not_to_be(std::ostream & os) const
{
  std::set<std::string> attr_names;
  for (auto attr : _attributes)
  {
    attr->to_be_or_not_to_be(os);
    if (!attr_names.insert(attr->name()).second)
    {
      os << "Non unique attribute name " << attr->name() << std::endl;
    }
  }
}

Element::child_list Element::children() const
{
  return {};
}

// vim:ft=cpp et sw=2 sts=2:
