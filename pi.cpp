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
  std::string s = "<?" + _target + " ";
  for (auto a : _data)
  {
    s += a->str() + " ";
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

void PI::to_be_or_not_to_be() const
{
    for (auto a : _data)
    {
        a->to_be_or_not_to_be();
    }
}
