#ifndef PI_H
#define PI_H

#include <string>
#include <vector>
#include "content.h"

class PI : public Content {
  public:
    typedef std::vector<std::string> data_list;

    PI(std::string const &, data_list const &);
    virtual ~PI();

    std::string const & target() const;
    data_list const & data() const;

  private:
    std::string _target;
    data_list _data;
};

#endif // vim:ft=cpp et sw=2 sts=2:
