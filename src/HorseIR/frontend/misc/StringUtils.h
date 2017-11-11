#pragma once

#include <string>
#include <functional>
#include <cctype>
#include <algorithm>

namespace horseIR
{
namespace misc
{

inline void trim_left (std::string &str)
{
  str.erase (str.begin (), std::find_if (
      str.begin (), str.end (),
      std::not1 (std::ptr_fun<int, int> (std::isspace))));
}

inline void trim_right (std::string &str)
{
  str.erase (std::find_if (
      str.rbegin (), str.rend (),
      std::not1 (std::ptr_fun<int, int> (std::isspace))).base (), str.end ());
}

inline void trim (std::string &str)
{
  trim_left (str);
  trim_right (str);
}

inline std::string trim_left_copy (std::string str)
{
  trim_left (str);
  return str;
}

inline std::string trim_right_copy (std::string str)
{
  trim_right (str);
  return str;
}

inline std::string trim_copy (std::string str)
{
  trim (str);
  return str;
}

}
}