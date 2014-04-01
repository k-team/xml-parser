#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>

#include "Content.h"

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
