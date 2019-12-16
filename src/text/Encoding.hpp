#pragma once
#ifndef TEXT_ENCODING_HPP
#define TEXT_ENCODING_HPP
#include <string>
namespace planet {

namespace Encoding {
std::string toMultiByteString(const std::wstring& input);
std::wstring toWideString(const std::string& input);
}  // namespace Encoding
}  // namespace planet
#endif