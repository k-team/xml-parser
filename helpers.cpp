#include "helpers.h"
#include <sstream>

namespace Helpers
{
  std::vector<std::string> & split(const std::string & s, char delim, std::vector<std::string> & elems)
  {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
      elems.push_back(item);
    }
    return elems;
  }

  std::vector<std::string> split(const std::string & s, char delim)
  {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
  }
}

// vim:ft=cpp et sw=2 sts=2:
