#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include "base.h"

class Attribute: public Base {
  public:
    Attribute(std::string const &, std::string const &);
    virtual ~Attribute();

    virtual std::string str() const;

    std::string const & name() const;
    std::string const & value() const;

  private:
    std::string _name;
    std::string _value;
};

#endif // vim:ft=cpp et sw=2 sts=2:
