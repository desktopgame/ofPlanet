#ifndef GEL_DEVICE_TEXTUREMANAGER_HPP
#define GEL_DEVICE_TEXTUREMANAGER_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include "../gli.hpp"
namespace gel {
class ITexture;
/**
 * TextureManager is manage a `ITexture`.
 * in now, supported .bmp and .png
 */
class TextureManager {
       public:
        TextureManager();
        /**
         * load a .bmp file.
         * @param path
         */
        void loadBmp(const std::string& path);
        /**
         * load a .png file
         * @param path
         */
        void loadPng(const std::string& path);
        /**
         * return a texture.
         * @param path
         * @return
         */
        std::shared_ptr<ITexture> getTexture(const std::string& path);
        /**
         * unload texture.
         * @param path
         */
        void unload(const std::string& path);

       private:
        std::unordered_map<std::string, std::shared_ptr<ITexture> > texMap;
};
}  // namespace gel
#endif