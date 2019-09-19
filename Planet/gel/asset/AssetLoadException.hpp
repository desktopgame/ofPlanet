#pragma once
#ifndef GEL_ASSET_ASSETLOADEXCEPTION_HPP
#define GEL_ASSET_ASSETLOADEXCEPTION_HPP
#include <stdexcept>

namespace gel {
class AssetLoadException : public std::exception {
public:
	explicit AssetLoadException(const std::string& message);
	static AssetLoadException createFromErrno();
};
}
#endif