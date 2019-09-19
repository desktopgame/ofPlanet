#include "AssetLoadException.hpp"
#include <cerrno>

namespace gel {
AssetLoadException::AssetLoadException(const std::string& message)
 : std::exception(message.c_str()) {
}
AssetLoadException AssetLoadException::createFromErrno()
{
	char* e = std::strerror(errno);
	return AssetLoadException(e);
}
}