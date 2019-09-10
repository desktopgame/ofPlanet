#pragma once
#ifndef GEL_ASSET_TEXTUREIO_HPP
#define GEL_ASSET_TEXTUREIO_HPP
#include <memory>
#include "ITexture.hpp"

namespace gel {
class TextureIO {
       public:
        static std::shared_ptr<ITexture> load(const std::string path);

       private:
        TextureIO() = delete;
        ~TextureIO() = delete;
};
}  // namespace gel
#endif