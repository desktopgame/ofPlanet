#include "string.hpp"
namespace gel {
std::vector<std::string> split(const std::string& input, const char c) {
        std::vector<std::string> ret;
        std::string buf;
        for (int i = 0; i < input.size(); i++) {
                char e = input[i];
                if (e == c) {
                        ret.push_back(buf);
                        buf.clear();
                } else {
                        buf.push_back(e);
                }
        }
        if (!buf.empty()) {
                ret.push_back(buf);
        }
        return ret;
}

std::string lstrip(const std::string& input) {
        std::string buf;
        for (int i = 0; i < input.size(); i++) {
                auto c = input[i];
                if (c != ' ' && c != '\t' && c != '\n') {
                        for (int j = i; j < input.size(); j++) {
                                buf.push_back(input[j]);
                        }
                        break;
                }
        }
        return buf;
}

std::string rstrip(const std::string& input) {
        std::string buf = input;
        while (!buf.empty()) {
                auto c = buf.back();
                if (c != ' ' && c != '\t' && c != '\n') {
                        break;
                }
                buf.pop_back();
        }
        return buf;
}

bool include(const std::string& input, const char c) {
        auto at = std::find(input.begin(), input.end(), c);
        return at != input.end();
}
}  // namespace gel