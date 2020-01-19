#include "File.hpp"

#include <fstream>
#include <sstream>
#include <cstdio>
#include "Path.hpp"

#if _WIN32
#include <Windows.h>
#endif

namespace planet {

std::string File::readAllText(const std::string& path) {
        std::ifstream ifs(path);
        if (ifs.fail()) {
                throw std::logic_error("Failed to open file.");
        }
        std::stringstream ss;
        std::string line;
        while (std::getline(ifs, line)) {
                ss << line << std::endl;
        }
        ifs.close();
        return ss.str();
}

void File::writeAllText(const std::string& path, const std::string& str) {
        std::ofstream ofs(path);
        ofs << str;
        ofs.close();
}
void File::remove(const std::string & path) {
	if (Path::isExists(path)) {
		::remove(path.c_str());
	}
}
void File::copy(const std::string & from, const std::string & to) {
	CopyFileA(from.c_str(), to.c_str(), TRUE);
}
}  // namespace planet
