#ifndef CHRDATA_H
#define CHRDATA_H

#include "Content.h"

class ChrData : public Content {
  public:
    ChrData(std::string const &);
    virtual ~ChrData();
};

#endif
