#ifndef GEL_UTIL_ERRROR_HPP
#define GEL_UTIL_ERRROR_HPP
#include <iostream>
#include <ostream>
#include <string>
namespace gel {
void checkError(const std::string& prefix, std::ostream& ofs);
void checkError(const std::string& prefix);
}  // namespace gel
#endif