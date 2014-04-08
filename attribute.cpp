#include "attribute.h"

Attribute::Attribute(Attribute::name_type const & n, Attribute::value_type const & v):
  _name(n), _value(v)
{
}

Attribute::~Attribute() {
}

Attribute::name_type const & Attribute::name() const {
  return _name;
}

Attribute::value_type const & Attribute::value() const {
  return _value;
}

// vim:ft=cpp et sw=2 sts=2:
