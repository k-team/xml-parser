#ifndef COMPOSITEELEMENT_H
#define COMPOSITEELEMENT_H

#include "element.h"

class CompositeElement : public Element {
  public:
    CompositeElement(std::vector<Content *> const &,
        std::vector<Attribute *> const &);
    virtual ~CompositeElement();

    std::vector<Content *> const & content() const;

  protected:
    std::vector<Content *> _content;
};

#endif
