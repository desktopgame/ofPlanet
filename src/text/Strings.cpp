#include "Strings.hpp"
namespace planet {

std::vector<std::string> Strings::split(const std::string& input,
                                        const char c) {
        std::vector<std::string> ret;
        std::string buf;
        for (int i = 0; i < static_cast<int>(input.size()); i++) {
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
        for (int i = 0; i < static_cast<int>(input.size()); i++) {
                auto c = input[i];
                if (c != ' ' && c != '\t' && c != '\n') {
                        for (int j = i; j < static_cast<int>(input.size());
                             j++) {
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
        for (int i = 0; i < static_cast<int>(input.size()); i++) {
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
bool Strings::hasPrefix(const std::string& input, const std::string& prefix) {
        if (prefix.size() > input.size()) {
                return false;
        }
        for (int i = 0; i < static_cast<int>(prefix.size()); i++) {
                if (prefix[i] != input[i]) {
                        return false;
                }
        }
        return true;
}
bool Strings::hasSuffix(const std::string& input, const std::string& suffix) {
        if (suffix.size() > input.size()) {
                return false;
        }
        for (int i = 0; i < static_cast<int>(suffix.size()); i++) {
                char sc = suffix[i];
                char ic = input[suffix.size() - i];
                if (sc != ic) {
                        return false;
                }
        }
        return true;
}
std::string Strings::fixprefix(const std::string& input,
                               const std::string& prefix) {
        if (hasPrefix(input, prefix)) {
                return input;
        }
        return prefix + input;
}
std::string Strings::fixsuffix(const std::string& input,
                               const std::string& suffix) {
        if (hasSuffix(input, suffix)) {
                return input;
        }
        return input + suffix;
}
}  // namespace planet
