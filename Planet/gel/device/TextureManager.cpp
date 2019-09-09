#include "TextureManager.hpp"
#include "BmpTexture.hpp"
#include "ITexture.hpp"
#include "JpegTexture.hpp"
#include "PngTexture.hpp"
namespace gel {
TextureManager::TextureManager() : texMap() {}

void TextureManager::loadBmp(const std::string& path) {
        auto bmp = std::make_shared<BmpTexture>();
        bmp->loadOnAsync(path);
        bmp->loadOnGLThread(path);
        texMap[path] = bmp;
}

void TextureManager::loadPng(const std::string& path) {
        auto png = std::make_shared<PngTexture>();
        png->loadOnAsync(path);
        png->loadOnGLThread(path);
        texMap[path] = png;
}

void TextureManager::loadJpeg(const std::string& path) {
        auto jpeg = std::make_shared<JpegTexture>();
        jpeg->loadOnAsync(path);
        jpeg->loadOnGLThread(path);
        texMap[path] = jpeg;
}

std::shared_ptr<ITexture> TextureManager::getTexture(const std::string& path) {
        if (texMap.count(path)) {
                return texMap.at(path);
        }
        return nullptr;
}

void TextureManager::unload(const std::string& path) {
        if (texMap.count(path)) {
                texMap.at(path)->unload();
        }
}
}  // namespace gel