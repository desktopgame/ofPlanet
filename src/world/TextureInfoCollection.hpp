#pragma once
#ifndef WORLD_TEXTURECOLLECTIONINFO_HPP
#define WORLD_TEXTURECOLLECTIONINFO_HPP
#include <string>
#include <vector>

#include "../mapbox/optional.hpp"

namespace planet {
struct TextureMappingRule {
        explicit TextureMappingRule() = default;
        mapbox::util::optional<std::string> all, top, bottom, left, right,
            front, back;
};

struct TextureInfo {
        explicit TextureInfo() = default;
        std::string baseFileName;
        std::string reference;
        TextureMappingRule mappingRule;
};

class TextureInfoCollection {
       public:
        explicit TextureInfoCollection(const std::string& baseDirectory);
        explicit TextureInfoCollection();

        std::string serialize() const;
        void deserialize(const std::string& json);

        void addTextureInfo(const TextureInfo& textureInfo);
        TextureInfo getTextureInfo(int index) const;
        mapbox::util::optional<TextureInfo> getTextureInfo(
            const std::string& reference) const;
        int getTextureInfoCount() const;

        std::string getBaseDirectory() const;

       private:
        std::vector<TextureInfo> textureInfoVec;
        std::string baseDirectory;
};
}  // namespace planet
#endif