#ifndef BASE_H
#define BASE_H

#include <string>
#include <sstream>

class Base {
  public:
    Base();
    virtual ~Base();

    virtual void to_be_or_not_to_be(std::ostream &) const;
    virtual std::string str() const;
};

#endif// vim:ft=cpp et sw=2 sts=2:
