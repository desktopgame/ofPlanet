#pragma once
#ifndef IO_PATH_HPP
#define IO_PATH_HPP
#include <string>
#include <vector>
namespace planet {

namespace Path {
std::string withoutExtension(const std::string& path);
std::string getFileNameFromPath(const std::string& path);
std::string getDirectoryPathFromPath(const std::string& path);
std::string slashToEscape(const std::string path);
std::string escapeToSlash(const std::string path);
std::string relative(const std::string& full, const std::string& base);
std::string build(std::vector<std::string> components);
std::string build(std::string car, std::vector<std::string> cdr);

std::string appendFirstSlash(const std::string& path);
std::string removeFirstSlash(const std::string& path);

std::string appendLastSlash(const std::string& path);
std::string removeLastSlash(const std::string& path);

bool isExists(const std::string& path);
bool isDirectory(const std::string& path);
bool isFile(const std::string& path);
}  // namespace Path
}  // namespace planet
#endif