#ifndef DOCTYPE_H
#define DOCTYPE_H

#include <string>

#include "base.h"

class Doctype : public Base {
  public:
    Doctype(std::string const &, std::string const &, std::string const &);
    virtual ~Doctype();

  private:
    std::string _n1;
    std::string _n2;
    std::string _n3;
};

#endif // vim:ft=cpp et sw=2 sts=2:
