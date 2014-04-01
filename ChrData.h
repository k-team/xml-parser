#ifndef CHRDATA_H
#define CHRDATA_H

#include "Content.h"

class ChrData : public Content {
  public:
    ChrData(std::string const &);
    virtual ~ChrData();

    std::string const & data() const;

  protected:
    std::string _data;
};

#endif
