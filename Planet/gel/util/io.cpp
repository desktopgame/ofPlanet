#include "io.hpp"
#include <fstream>
#include <sstream>
namespace gel {
std::string getDirectoryFromPath(const std::string & path, const char separator) {
	using str_size = std::string::size_type;
	std::string cp = path;
	str_size len = path.size() - 1;
	while (len > 0) {
		char c = cp.at(len);
		if (c == separator) {
			break;
		}
		cp.pop_back();
		len--;
	}
	return cp;
}
	bool exists(const std::string& path) {
        FILE* fp;
        errno_t err = fopen_s(&fp, path.c_str(), "r");
        if (err == 0) {
                fclose(fp);
        }
        return err == 0;
}
std::string readAllText(const std::string& path) {
        std::ifstream ifs(path);
        std::stringstream ss;
        std::string line;
        while (std::getline(ifs, line)) {
                ss << line << std::endl;
        }
        ifs.close();
        return ss.str();
}

void writeAllText(const std::string& path, const std::string& str) {
        std::ofstream ofs(path);
        ofs << str;
        ofs.close();
}
}  // namespace gel