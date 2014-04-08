#ifndef CHARDATA_H
#define CHARDATA_H

#include <string>
#include "content.h"

class CharData: public Content {
  public:
    CharData(std::string const &);
    virtual ~CharData();

    std::string const & data() const;
    std::string str() const;

  private:
    std::string _data;
};

#endif // vim:ft=cpp et sw=2 sts=2:
