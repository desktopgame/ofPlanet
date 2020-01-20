#pragma once
#ifndef TEXT_ENCODING_HPP
#define TEXT_ENCODING_HPP
#include <string>
namespace planet {

namespace Encoding {
/**
 * ワイド文字列をマルチバイト文字列へ変換します。
 * @param input
 * @return
 */
std::string toMultiByteString(const std::wstring& input);
/**
 * マルチバイト文字列をワイド文字列へ変換します。
 * @param input
 * @return
 */
std::wstring toWideString(const std::string& input);
}  // namespace Encoding
}  // namespace planet
#endif