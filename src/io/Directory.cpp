#include "Directory.hpp"

#include "../text/Encoding.hpp"
#include "Path.hpp"
namespace planet {

std::vector<std::string> Directory::files(const std::string& path,
                                          bool recursive) {
        return Directory::hidden::entries(
            path, recursive, [](std::string file, DWORD attr) -> bool {
                    return (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
            });
}

std::vector<std::string> Directory::directories(const std::string& path,
                                                bool recursive) {
        return Directory::hidden::entries(
            path, recursive, [](std::string file, DWORD attr) -> bool {
                    return (attr & FILE_ATTRIBUTE_DIRECTORY) > 0;
            });
}

std::vector<std::string> Directory::entries(const std::string& path,
                                            bool recursive) {
        return Directory::hidden::entries(
            path, recursive,
            [](std::string file, DWORD attr) -> bool { return true; });
}
namespace Directory {
namespace hidden {
std::vector<std::string> entries(
    const std::string& path, bool recursive,
    std::function<bool(std::string, DWORD)> filter) {
        std::string fixedPath = Path::slashToEscape(path);
        std::vector<std::string> ret;
        HANDLE hFind;
        WIN32_FIND_DATA win32fd;
        std::string search_name = fixedPath + "\\*";

        hFind = FindFirstFile(Encoding::toWideString(search_name).c_str(),
                              &win32fd);
        if (hFind == INVALID_HANDLE_VALUE) {
                return ret;
        }
        std::vector<std::string> dirs;
        do {
                std::string filename =
                    Encoding::toMultiByteString(win32fd.cFileName);
                if (filename == "." || filename == "..") {
                        continue;
                }
                std::string file = path + '/' + filename;
                if (filter(file, win32fd.dwFileAttributes)) {
                        ret.push_back(file);
                }
                if ((win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0 &&
                    recursive) {
                        dirs.emplace_back(file);
                }
        } while (FindNextFile(hFind, &win32fd));
        FindClose(hFind);
        if (recursive) {
                for (auto dir : dirs) {
                        std::vector<std::string> children =
                            entries(dir, recursive, filter);
                        for (auto child : children) {
                                ret.emplace_back(child);
                        }
                }
        }
        return ret;
}
}  // namespace hidden
}  // namespace Directory
}  // namespace planet
