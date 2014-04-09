#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>

#include "base.h"

class Prolog;
class Element;
class PI;

class Document: public Base
{
  public:
    Document(Prolog *, Element *, std::vector<PI *> const &);
    virtual ~Document();

    Prolog * prolog() const;
    Element * root() const;
    std::vector<PI *> const & endPIs() const;

    std::string str() const;
    void to_be_or_not_to_be(std::ostream &) const;

  private:
    Prolog * _prolog;
    Element * _root;
    std::vector<PI *> _endPIs;
};

#endif // vim:ft=cpp et sw=2 sts=2:
