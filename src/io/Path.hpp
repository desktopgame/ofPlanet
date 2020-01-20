#pragma once
#ifndef IO_PATH_HPP
#define IO_PATH_HPP
#include <string>
#include <vector>
namespace planet {

namespace Path {
/**
 * パスから拡張子を取り除いて返します。
 * @param path
 * @return
 */
std::string withoutExtension(const std::string& path);

/**
 * パスからファイル名を取り出して返します。
 * @param path
 * @return
 */
std::string getFileNameFromPath(const std::string& path);

/**
 * パスからディレクトリ名を取り出して返します。
 * @param path
 * @return
 */
std::string getDirectoryPathFromPath(const std::string& path);

/**
 * パスの区切り文字 / をバックスラッシュへ変換して返します。
 * @param path
 * @return
 */
std::string slashToEscape(const std::string path);

/**
 * パスの区切り文字 バックスラッシュ を / へ変換して返します。
 * @param path
 * @return
 */
std::string escapeToSlash(const std::string path);

/**
 * 絶対パス full を base から見た相対パスへ変換します。
 * @param ful
 * @param base
 * @return
 */
std::string relative(const std::string& full, const std::string& base);

/**
 * パスの一覧を全て接続して返します。
 * @param components
 * @return
 */
std::string build(std::vector<std::string> components);

/**
 * パスの一覧を全て接続して返します。
 * @param car
 * @param cdr
 * @return
 */
std::string build(std::string car, std::vector<std::string> cdr);

/**
 * パスの先頭にスラッシュを追加して返します。
 * @param path
 * @return
 */
std::string appendFirstSlash(const std::string& path);

/**
 * パスの先頭からスラッシュを取り除いて返します。
 * @param path
 * @return
 */
std::string removeFirstSlash(const std::string& path);

/**
 * パスの末尾にスラッシュを追加して返します。
 * @param path
 * @return
 */
std::string appendLastSlash(const std::string& path);

/**
 * パスの末尾にスラッシュを追加して返します。
 * @param path
 * @return
 */
std::string removeLastSlash(const std::string& path);

/**
 * 指定のパスが存在するなら true を返します。
 * @param path
 * @return
 */
bool isExists(const std::string& path);

/**
 * 指定のパスがディレクトリなら true を返します。
 * @param path
 * @return
 */
bool isDirectory(const std::string& path);

/**
 * 指定のパスがディレクトリなら false を返します。
 * @param path
 * @return
 */
bool isFile(const std::string& path);
}  // namespace Path
}  // namespace planet
#endif