#include "Strings.hpp"
namespace planet {

std::vector<std::string> Strings::split(const std::string& input,
                                        const char c) {
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

std::string Strings::lstrip(const std::string& input) {
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

std::string Strings::rstrip(const std::string& input) {
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

bool Strings::include(const std::string& input, const char c) {
        auto at = std::find(input.begin(), input.end(), c);
        return at != input.end();
}

std::string Strings::replace(const std::string& input, char oldc, char newc) {
        std::string buf;
        for (int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                if (c == oldc) {
                        buf += newc;
                } else {
                        buf += c;
                }
        }
        return buf;
}

std::string Strings::replace(const std::string target, const std::string from,
                             const std::string to) {
        std::string result = target;
        std::string::size_type pos = 0;
        while (pos = result.find(from, pos), pos != std::string::npos) {
                result.replace(pos, from.length(), to);
                pos += to.length();
        }
        return result;
}
}  // namespace planet
