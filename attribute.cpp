#include "attribute.h"

Attribute::Attribute(std::string const & name, std::string const & value):
  _name(name), _value(value)
{
}

Attribute::~Attribute()
{
}

std::string Attribute::str() const
{
  return _name + "=\"" + _value + "\"";
}

std::string const & Attribute::name() const
{
  return _name;
}

std::string const & Attribute::value() const
{
  return _value;
}

// vim:ft=cpp et sw=2 sts=2:
