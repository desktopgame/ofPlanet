#include "io.hpp"
#include <fstream>
#include <sstream>
namespace gel {
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