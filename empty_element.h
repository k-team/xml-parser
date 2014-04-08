#ifndef EMPTYELEMENT_H
#define EMPTYELEMENT_H

#include "element.h"

class EmptyElement : public Element {
  public:
    EmptyElement(std::vector<Attribute *> const &);
    virtual ~EmptyElement();
};

#endif
