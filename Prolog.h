#ifndef PROLOG_H
#define PROLOG_H

#include <vector>

class XMLDecl;
class PI;
class Doctype;

class Prolog {
  public:
    Prolog(XMLDecl *, std::vector<PI *> const &, Doctype *, std::vector<PI *> const &);
    virtual ~Prolog();
};

#endif
