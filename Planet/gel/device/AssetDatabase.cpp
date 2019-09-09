#include "AssetDatabase.hpp"

std::unordered_map<std::string, std::shared_ptr<gel::IAsset> >
    gel::AssetDatabase::assets =
        std::unordered_map<std::string, std::shared_ptr<gel::IAsset> >();

void gel::AssetDatabase::addAsset(const std::string& path,
                                  const std::shared_ptr<IAsset>& asset) {
        assets[path] = asset;
}
