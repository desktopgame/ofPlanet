#include "TextureManager.hpp"
#include "BmpTexture.hpp"
#include "ITexture.hpp"
#include "PngTexture.hpp"
namespace gel {
TextureManager::TextureManager() : texMap() {}

void TextureManager::loadBmp(const std::string& path) {
        auto bmp = std::make_shared<BmpTexture>();
        bmp->load(path);
        texMap[path] = bmp;
}

void TextureManager::loadPng(const std::string& path) {
        auto png = std::make_shared<PngTexture>();
        png->load(path);
        texMap[path] = png;
}

std::shared_ptr<ITexture> TextureManager::getTexture(const std::string& path) {
        if (texMap.count(path)) {
                return texMap.at(path);
        }
        return nullptr;
}

void TextureManager::unload(const std::string& path) {
        if (texMap.count(path)) {
                texMap.at(path)->unload(path);
        }
}
}  // namespace gel