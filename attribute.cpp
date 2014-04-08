#include "attribute.h"

Attribute::Attribute(name_type const & n, value_type const & v):
  _name(n), _value(v)
{
}

Attribute::~Attribute() {
}

name_type const & Attribute::name() const {
  return _name;
}

value_type const & Attribute::value() const {
  return _value;
}
