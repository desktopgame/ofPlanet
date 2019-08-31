#ifndef GEL_UTIL_STRING_HPP
#define GEL_UTIL_STRING_HPP
#include <string>
#include <vector>

namespace gel {
std::vector<std::string> split(const std::string& input, const char c);

std::string lstrip(const std::string& input);

std::string rstrip(const std::string& input);

bool include(const std::string& input, const char c);
}  // namespace gel
#endif