#ifndef GEL_UTIL_IO_HPP
#define GEL_UTIL_IO_HPP
#include <string>
namespace gel {
std::string readAllText(const std::string& path);

void writeAllText(const std::string& path, const std::string& str);
}  // namespace gel
#endif