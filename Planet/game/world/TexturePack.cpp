#include "TexturePack.hpp"
namespace gel {}

TexturePack::TexturePack() {}

TexturePack TexturePack::side1(
    const std::shared_ptr<gel::TextureManager>& textureManager,
    const std::string& path) {
        auto tid = textureManager->getTexture(path)->getID();
        TexturePack p;
        p.posXTex = tid;
        p.posYTex = tid;
        p.posZTex = tid;
        p.negXTex = tid;
        p.negYTex = tid;
        p.negZTex = tid;
        return p;
}

TexturePack TexturePack::side3(
    const std::shared_ptr<gel::TextureManager>& textureManager,
    const std::string& prefix, const std::string& suffix) {
        auto topID =
            textureManager->getTexture(prefix + "Top" + suffix)->getID();
        auto sideID =
            textureManager->getTexture(prefix + "Side" + suffix)->getID();
        auto bottomID =
            textureManager->getTexture(prefix + "Bottom" + suffix)->getID();
        TexturePack p;
        p.posXTex = sideID;
        p.posYTex = topID;
        p.posZTex = sideID;
        p.negXTex = sideID;
        p.negYTex = bottomID;
        p.negZTex = sideID;
        return p;
}

TexturePack TexturePack::side6(
    const std::shared_ptr<gel::TextureManager>& textureManager,
    const std::string& prefix, const std::string& suffix) {
        return TexturePack();
}
