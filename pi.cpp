#include "pi.h"

#include "attribute.h"

PI::PI(std::string const & target, PI::data_list const & data):
    _target(target), _data(data)
{
}

PI::~PI()
{
}

std::string PI::str() const
{
  std::string s = "<?" + _target.str() + " ";
  for (std::vector<Attribute *>::iterator it(_data.begin());
      it != _data.end(); it++) {
    s += it->str() + " ";
  }
  return s + "?>";
}

std::string const & PI::target() const
{
    return _target;
}

PI::data_list const & PI::data() const
{
    return _data;
}
