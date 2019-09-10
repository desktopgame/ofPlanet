#pragma once
#ifndef GEL_ASSET_IASSET_HPP
#define GEL_ASSET_IASSET_HPP
#include <string>
#include "../Thread.hpp"

namespace gel {
class IAsset {
       public:
        IAsset() = default;
        virtual ~IAsset() = default;
        virtual void load(const std::string& path, Thread thread) = 0;
        virtual void unload() = 0;
};
}  // namespace gel
#endif