#pragma once
#ifndef IO_FILE_HPP
#define IO_FILE_HPP
#include <string>
namespace planet {

namespace File {
std::string readAllText(const std::string& path);
void writeAllText(const std::string& path, const std::string& str);
void remove(const std::string& path);
void copy(const std::string& from, const std::string& to);
}  // namespace File
}  // namespace planet
#endif