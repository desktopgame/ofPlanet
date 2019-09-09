#pragma once
#ifndef GEL_DEVICE_IASSET_HPP
#define GEL_DEVICE_IASSET_HPP
#include <string>

namespace gel {
class IAsset {
       public:
        IAsset() = default;
        virtual ~IAsset() = default;
        virtual void loadOnAsync(const std::string& path) = 0;
        virtual void loadOnGLThread(const std::string& path) = 0;
        virtual void unload() = 0;
};
}  // namespace gel
#endif