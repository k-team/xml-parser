#ifndef COMPOSITEELEMENT_H
#define COMPOSITEELEMENT_H

#include "element.h"

class Attribute;
class Content;

class CompositeElement: public Element {
  public:
    typedef std::vector<Content *> content_list;
    CompositeElement(std::string const &, std::string const &,
        Element::attribute_list const &, content_list const &);
    virtual ~CompositeElement();

    content_list const & content() const;
    std::string const & begin_tag() const;
    std::string const & end_tag() const;

    std::string str() const;

  private:
    std::string _end;
    content_list _content;
};

#endif // vim:ft=cpp et sw=2 sts=2:
