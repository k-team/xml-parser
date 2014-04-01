#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>

class Attribute {
  public:
    typedef std::string name_type;
    typedef std::string value_type;

    Attribute(name_type const &, value_type const &);
    virtual ~Attribute();
};

#endif

