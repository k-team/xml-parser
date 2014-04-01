#ifndef CDSECT_H
#define CDSECT_H

#include "Content.h"

class CDSect : public Content {
  public:
    CDSect(std::string const &);
    virtual ~CDSect();

    std::string const & sect() const;

  protected:
    std::string const & _sect;
};

#endif

