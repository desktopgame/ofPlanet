#pragma once
#ifndef IO_DIRECTORY_HPP
#define IO_DIRECTORY_HPP
#include <Windows.h>

#include <functional>
#include <string>
#include <vector>
namespace planet {

namespace Directory {
/**
 * 指定のディレクトリのファイル一覧を返します。
 * @param path
 * @param recursive
 * @return
 */
std::vector<std::string> files(const std::string& path, bool recursive);

/**
 * 指定のディレクトリのサブディレクトリ一覧を返します。
 * @param path
 * @param recursive
 * @return
 */
std::vector<std::string> directories(const std::string& path, bool recursive);

/**
 * 指定のディレクトリのファイル, サブディレクトリ一覧を返します。
 * @param path
 * @param recursive
 * @return
 */
std::vector<std::string> entries(const std::string& path, bool recursive);

/**
 * 指定のディレクトリを作成します。
 * @param dir
 */
void create(const std::string& dir);

namespace hidden {
std::vector<std::string> entries(
    const std::string& path, bool recursive,
    std::function<bool(std::string, DWORD)> filter);
}
}  // namespace Directory
}  // namespace planet
#endif