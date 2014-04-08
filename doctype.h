#ifndef DOCTYPE_H
#define DOCTYPE_H

#include "base.h"

class Doctype : public Base {
  public:
    Doctype(std::string const &, std::string const &, std::string const &);
    virtual ~Doctype();

  private:
    // TODO
};

#endif // vim:ft=cpp et sw=2 sts=2:
