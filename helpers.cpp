#include "helpers.h"
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

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

  std::string & ltrim(std::string & s)
  {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
          std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  }

  std::string & rtrim(std::string & s)
  {
    s.erase(std::find_if(s.rbegin(), s.rend(),
          std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
  }

  std::string & trim(std::string & s)
  {
    return ltrim(rtrim(s));
  }

  std::string & lower(std::string & s)
  {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
  }
}

// vim:ft=cpp et sw=2 sts=2:
