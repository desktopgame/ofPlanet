#include "Encoding.hpp"

#include <codecvt>
namespace planet {

std::string Encoding::toMultiByteString(const std::wstring& input) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
        return cv.to_bytes(input);
}

std::wstring Encoding::toWideString(const std::string& input) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
        return cv.from_bytes(input);
}
}  // namespace planet
