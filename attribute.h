#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>

class Attribute {
  public:
    typedef std::string name_type;
    typedef std::string value_type;

    Attribute(name_type const &, value_type const &);
    virtual ~Attribute();

    name_type const & name() const;
    value_type const & value() const;

  protected:
    name_type _name;
    value_type _value;
};

#endif

// vim:ft=cpp et sw=2 sts=2: