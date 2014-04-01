#include "CompositeElement.h"
#include "Attribute.h"

CompositeElement::CompositeElement(std::vector<Content *> const & c,
    std::vector<Attribute *> const & a):
  Element(a),
  _content(c)
{
}

CompositeElement::~CompositeElement() {
  for (std::vector<Content *>::iterator it = _content.begin();
      it != _content.end(); it++)
    delete *it;
}

std::vector<Content *> const & CompositeElement::content() const {
  return _content;
}
