#ifndef COMPOSITEELEMENT_H
#define COMPOSITEELEMENT_H

#include "Element.h"

class CompositeElement : public Element {
  public:
    CompositeElement(std::vector<Content *> const &,
        std::vector<Attribute *> const &);
    virtual ~CompositeElement();
};

#endif
