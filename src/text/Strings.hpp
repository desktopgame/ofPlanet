#pragma once
#ifndef TEXT_STRINGS_HPP
#define TEXT_STRINGS_HPP
#include <string>
#include <vector>
namespace planet {

namespace Strings {
/**
 * 文字列を任意の文字で分割します。
 * @param input
 * @param c
 * @return
 */
std::vector<std::string> split(const std::string& input, const char c);

/**
 * 左のスペースを削除します。
 * @param input
 * @return
 */
std::string lstrip(const std::string& input);

/**
 * 右のスペースを削除します。
 * @param input
 * @return
 */
std::string rstrip(const std::string& input);

/**
 * 任意の文字が含まれるなら true を返します。
 * @param input
 * @param c
 * @return
 */
bool include(const std::string& input, const char c);

/**
 * 文字列中の文字を置き換えます。
 * @param input
 * @param oldc
 * @param newc
 * @return
 */
std::string replace(const std::string& input, char oldc, char newc);

/**
 * 文字列中の文字列を置き換えます。
 * @param input
 * @param from
 * @param to
 * @return
 */
std::string replace(const std::string target, const std::string from,
                    const std::string to);

/**
 * 指定のプレフィックスを持つなら true を返します。
 * @param input
 * @param prefix
 */
bool hasPrefix(const std::string& input, const std::string& prefix);

/**
 * 指定のサフィックスを持つなら true を返します。
 * @param input
 * @param suffix
 */
bool hasSuffix(const std::string& input, const std::string& suffix);

/**
 * プレフィックスを付け足します。
 * @param input
 * @param pefix
 * @return
 */
std::string fixprefix(const std::string& input, const std::string& prefix);

/**
 * サフィックスを付け足します。
 * @param input
 * @param suffix
 */
std::string fixsuffix(const std::string& input, const std::string& suffix);
}  // namespace Strings
}  // namespace planet
#endif