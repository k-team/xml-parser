#include "attribute.h"

Attribute::Attribute(Attribute::name_type const & name,
    Attribute::value_type const & value):
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

Attribute::name_type const & Attribute::name() const
{
  return _name;
}

Attribute::value_type const & Attribute::value() const
{
  return _value;
}

// vim:ft=cpp et sw=2 sts=2:
