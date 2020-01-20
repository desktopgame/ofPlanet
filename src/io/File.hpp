#pragma once
#ifndef IO_FILE_HPP
#define IO_FILE_HPP
#include <string>
namespace planet {

namespace File {
/**
 * 指定のパスのテキストを全て読み込んで返します。
 * @param path
 * @return
 */
std::string readAllText(const std::string& path);

/**
 * 指定のファイルにテキストを全て書き込みます。
 * @param path
 * @param str
 */
void writeAllText(const std::string& path, const std::string& str);

/**
 * 指定のパスのファイルを削除します。
 * @param path
 */
void remove(const std::string& path);

/**
 * fromからtoへファイルを複製します。
 * @param from
 * @param to
 */
void copy(const std::string& from, const std::string& to);
}  // namespace File
}  // namespace planet
#endif