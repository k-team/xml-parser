#ifndef CDSECT_H
#define CDSECT_H

#include <string>
#include "content.h"

class CDSect : public Content {
  public:
    CDSect(std::string const &);
    virtual ~CDSect();

    virtual std::string str() const;

    std::string const & sect() const;

  protected:
    std::string _sect;
};

#endif // vim:ft=cpp et sw=2 sts=2:
