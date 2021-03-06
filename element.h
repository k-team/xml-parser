#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>

#include "content.h"

class Attribute;

class Element: public Content
{
  public:
    typedef std::vector<Attribute *> attribute_list;
    typedef std::vector<Content *> child_list;

    Element(std::string const &, Element::attribute_list const &);
    virtual ~Element();

    Attribute * find_attribute(std::string const &) const;

    std::string const & name() const;
    attribute_list const & attributes() const;

    std::pair<std::string, std::string> ns_split() const;

    virtual std::string str() const;
    virtual void to_be_or_not_to_be(std::ostream &) const;
    virtual child_list children() const;

  private:
    std::string _name;
    attribute_list _attributes;
};

#endif // vim:ft=cpp et sw=2 sts=2:
