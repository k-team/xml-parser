#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "base.h"

class Prolog;
class Element;
class PI;

class Document: public Base {
  public:
    Document(Prolog *, Element *, PI *);
    virtual ~Document();

    Prolog * prolog() const;
    Element * root() const;
    PI * endPI() const;

  protected:
    Prolog * _prolog;
    Element * _root;
    PI * _endPI;
};

#endif // vim:ft=cpp et sw=2 sts=2:
