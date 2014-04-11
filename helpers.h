#ifndef HELPERS_H
#define HELPERS_H

#include <vector>
#include <string>

namespace Helpers {
  std::vector<std::string> & split(const std::string & s, char delim,
      std::vector<std::string> & elems);
  std::vector<std::string> split(const std::string & s, char delim);
}

#endif // vim:ft=cpp et sw=2 sts=2:
