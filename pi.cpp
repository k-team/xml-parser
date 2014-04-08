#include "pi.h"

#include "attribute.h"

PI::PI(std::string const & target, PI::data_list const & data):
    _target(target), _data(data)
{
}

PI::~PI()
{
}

std::string const & PI::target() const
{
    return _target;
}

PI::data_list const & PI::data() const
{
    return _data;
}
