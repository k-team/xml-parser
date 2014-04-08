#ifndef BASE_H
#define BASE_H

class Base {
  public:
    Base();
    virtual ~Base();
    virtual bool to_be_or_not_to_be() = 0;
};

#endif // vim:ft=cpp et sw=2 sts=2:
