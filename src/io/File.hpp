#pragma once
#ifndef IO_FILE_HPP
#define IO_FILE_HPP
#include <string>
namespace planet {

namespace File {
std::string readAllText(const std::string& path);
void writeAllText(const std::string& path, const std::string& str);
void remove(const std::string& path);
}  // namespace File
}  // namespace planet
#endif