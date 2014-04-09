#ifndef PROLOG_H
#define PROLOG_H

#include "base.h"
#include <vector>

class PI;
class Doctype;

class Prolog: public Base {
  public:
    Prolog(std::vector<PI *> const &,
        Doctype *,
        std::vector<PI *> const &);
    virtual ~Prolog();

    virtual std::string str() const;

    std::vector<PI *> const & firstPIs() const;
    Doctype * doctype() const;
    std::vector<PI *> const & lastPIs() const;

    void to_be_or_not_to_be(std::ostream &) const;

  private:
    std::vector<PI *> _firstPIs;
    Doctype * _doctype;
    std::vector<PI *> _lastPIs;
};

#endif // vim:ft=cpp et sw=2 sts=2:
