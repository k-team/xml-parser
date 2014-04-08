#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>

#include "content.h"

class Attribute;

class Element {
  public:
    Element(std::vector<Attribute *> const &);
    virtual ~Element();

    std::vector<Attribute *> const & attributes() const;

  protected:
    std::vector<Attribute *> _attributes;
};

#endif

// vim:ft=cpp et sw=2 sts=2:
