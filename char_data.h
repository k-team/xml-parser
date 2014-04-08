#ifndef CHARDATA_H
#define CHARDATA_H

#include "content.h"

class CharData : public Content {
  public:
    CharData(std::string const &);
    virtual ~CharData();

    std::string const & data() const;

  protected:
    std::string _data;
};

#endif
