#ifndef BASE_H
#define BASE_H


class Base {
  public:
    Base();
    virtual ~Base();

    virtual void to_be_or_not_to_be() {};
    virtual void print() {};
};

#endif// vim:ft=cpp et sw=2 sts=2:
