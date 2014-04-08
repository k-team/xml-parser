#ifndef PROLOG_H
#define PROLOG_H

#include "base.h"
#include <vector>

class XMLDecl;
class PI;
class Doctype;

class Prolog: public Base {
  public:
    Prolog(XMLDecl *,
        std::vector<PI *> const &,
        Doctype *,
        std::vector<PI *> const &);
    virtual ~Prolog();

    XMLDecl * xmlDecl() const;
    std::vector<PI *> const & firstPIs() const;
    Doctype * doctype() const;
    std::vector<PI *> const & lastPIs() const;

  protected:
    XMLDecl * _xmlDecl;
    std::vector<PI *> _firstPIs;
    Doctype * _doctype;
    std::vector<PI *> _lastPIs;
};

#endif

// vim:ft=cpp et sw=2 sts=2:
