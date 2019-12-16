#include "File.hpp"

#include <fstream>
#include <sstream>
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
}  // namespace planet
