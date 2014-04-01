#ifndef CDSECT_H
#define CDSECT_H

#include "Content.h"

class CDSect : public Content {
  public:
    CDSect(std::string const &);
    virtual ~CDSect();
};

#endif

