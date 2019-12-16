#include "Path.hpp"

#include <Shlwapi.h>
#include <Windows.h>

#include <stdexcept>

#include "../text/Encoding.hpp"
namespace planet {
#pragma comment(lib, "shlwapi.lib")

std::string Path::withoutExtension(const std::string& path) {
        std::string::size_type pos = path.rfind(".");
        if (pos == std::string::npos) {
                return path;
        }
        return path.substr(0, pos);
}

std::string Path::getFileNameFromPath(const std::string& path) {
        std::string fixedPath = escapeToSlash(path);
        std::string::size_type pos = fixedPath.rfind('/');
        if (pos == std::string::npos) {
                return path;
        }
        return removeFirstSlash(fixedPath.substr(pos));
}

std::string Path::getDirectoryPathFromPath(const std::string& path) {
        std::string fixedPath = escapeToSlash(path);
        std::string::size_type pos = fixedPath.rfind('/');
        if (pos == std::string::npos) {
                return path;
        }
        return fixedPath.substr(0, pos);
}

std::string Path::slashToEscape(const std::string path) {
        std::string buf;
        for (auto c : path) {
                if (c == '/') {
                        c = '\\';
                }
                buf += c;
        }
        return buf;
}

std::string Path::escapeToSlash(const std::string path) {
        std::string buf;
        for (auto c : path) {
                if (c == '\\') {
                        c = '/';
                }
                buf += c;
        }
        return buf;
}

std::string Path::relative(const std::string& full, const std::string& base) {
        std::string fixedFull = Path::escapeToSlash(full);
        std::string fixedBase = Path::escapeToSlash(base);
        int fixedBaseSize = static_cast<int>(fixedBase.size());
        if (fixedFull.substr(0, fixedBaseSize) != fixedBase) {
                throw std::logic_error("invalid path:" + full);
        }
        std::string sub = fixedFull.substr(fixedBaseSize);
        if (sub[0] == '/') {
                return "." + sub;
        }
        return "./" + sub;
}

std::string Path::build(std::vector<std::string> components) {
        if (components.empty()) {
                throw std::logic_error("invalid components");
        }
        if (components.size() == 1) {
                return components.at(0);
        }
        return build(
            components[0],
            std::vector<std::string>(components.begin() + 1, components.end()));
}

std::string Path::build(std::string car, std::vector<std::string> cdr) {
        if (cdr.empty()) {
                return car;
        }
        return removeLastSlash(car) + '/' +
               removeFirstSlash(build(cdr[0], std::vector<std::string>(
                                                  cdr.begin() + 1, cdr.end())));
}

std::string Path::appendFirstSlash(const std::string& path) {
        if (path.at(0) == '/') {
                return path;
        }
        return '/' + path;
}

std::string Path::removeFirstSlash(const std::string& path) {
        if (path.at(0) == '/') {
                return path.substr(1);
        }
        return path;
}

std::string Path::appendLastSlash(const std::string& path) {
        int pathSize = static_cast<int>(path.size());
        char c = path.at(pathSize - 1);
        if (c == '/') {
                return path;
        }
        return path + '/';
}

std::string Path::removeLastSlash(const std::string& path) {
        int pathSize = static_cast<int>(path.size());
        char c = path.at(pathSize - 1);
        if (c == '/') {
                return path.substr(0, pathSize - 1);
        }
        return path;
}

bool Path::isExists(const std::string& path) {
        return PathFileExists(Encoding::toWideString(path).c_str()) == TRUE;
}

bool Path::isDirectory(const std::string& path) {
        DWORD mask = GetFileAttributes(Encoding::toWideString(path).c_str());
        if (mask == 0xFFFFFFFF) {
                throw std::logic_error("path is was not read");
        }
        return (mask & FILE_ATTRIBUTE_DIRECTORY) > 0;
}

bool Path::isFile(const std::string& path) { return !isDirectory(path); }
}  // namespace planet
