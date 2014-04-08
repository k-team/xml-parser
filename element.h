#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>

#include "content.h"

class Attribute;

class Element: public Content {
  public:
    typedef std::vector<Attribute *> attribute_list;

    // Construct an element, taking its tag name and the begin/end iterators
    // for its attributes. These must dereference to Attribute*.
    Element(std::string const &, Element::attribute_list const &);
    virtual ~Element();

    std::string const & name() const;
    attribute_list const & attributes() const;

  private:
    std::string _name;
    attribute_list _attributes;
};

#endif // vim:ft=cpp et sw=2 sts=2:
