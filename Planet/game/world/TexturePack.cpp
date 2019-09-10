#include "TexturePack.hpp"
#include "../../gel/asset/AssetDatabase.hpp"
namespace gel {}

TexturePack::TexturePack() {}

TexturePack TexturePack::side1(const std::string& path) {
        auto tid = gel::AssetDatabase::getAsset<gel::ITexture>(path)->getID();
        TexturePack p;
        p.posXTex = tid;
        p.posYTex = tid;
        p.posZTex = tid;
        p.negXTex = tid;
        p.negYTex = tid;
        p.negZTex = tid;
        return p;
}

TexturePack TexturePack::side3(const std::string& prefix,
                               const std::string& suffix) {
        auto topID =
            gel::AssetDatabase::getAsset<gel::ITexture>(prefix + "Top" + suffix)
                ->getID();
        auto sideID = gel::AssetDatabase::getAsset<gel::ITexture>(
                          prefix + "Side" + suffix)
                          ->getID();
        auto bottomID = gel::AssetDatabase::getAsset<gel::ITexture>(
                            prefix + "Bottom" + suffix)
                            ->getID();
        TexturePack p;
        p.posXTex = sideID;
        p.posYTex = topID;
        p.posZTex = sideID;
        p.negXTex = sideID;
        p.negYTex = bottomID;
        p.negZTex = sideID;
        return p;
}

TexturePack TexturePack::side6(const std::string& prefix,
                               const std::string& suffix) {
        return TexturePack();
}
