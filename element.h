#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>

#include "content.h"

class Attribute;

class Element: public Content {
  public:
    typedef std::vector<Attribute *> attribute_list;

    Element(std::string const &, Element::attribute_list const &);
    virtual ~Element();

    std::string const & name() const;
    attribute_list const & attributes() const;

    virtual std::string str() const;
    virtual void to_be_or_not_to_be() const;

  private:
    std::string _name;
    attribute_list _attributes;
};

#endif // vim:ft=cpp et sw=2 sts=2:
