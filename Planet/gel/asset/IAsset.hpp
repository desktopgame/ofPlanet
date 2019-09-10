#pragma once
#ifndef GEL_ASSET_IASSET_HPP
#define GEL_ASSET_IASSET_HPP
#include <string>
#include "../Thread.hpp"

namespace gel {
/**
 * IAsset is interface every resource in gel library.
 * implement resource load/unload system.
 * asset load system in gel library is load by two step
 *
 * - step1
 * load at another thread.
 * invoke load() pass Thread::OnBackground into parameter `thread`
 * in this, onUpdate, onDraw is called on parallel at OpenGL Thread
 * - step2
 * load at OpenGL thread.
 * invoke load() pass Thread::OnGL into parameter `thread`
 * in this, onUpdate, onDraw is not be called
 */
class IAsset {
       public:
        IAsset() = default;
        virtual ~IAsset() = default;
        virtual void load(const std::string& path, Thread thread) = 0;
        virtual void unload() = 0;
};
}  // namespace gel
#endif