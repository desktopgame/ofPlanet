#pragma once
#ifndef WORLD_TEXTUREPACK_HPP
#define WORLD_TEXTUREPACK_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../shader/Texture.hpp"
#include "TextureSet.hpp"

#include "TextureInfoCollection.hpp"

namespace planet {

class Plane;
class TexturePack : public std::enable_shared_from_this<TexturePack> {
       public:
		static std::shared_ptr<TexturePack> load(const TextureInfoCollection& textureInfoCollection);
        static std::shared_ptr<TexturePack> make(const std::string& name);
        static std::shared_ptr<TexturePack> getCurrent();

        void addTextureSet(const TextureSet& set);
        int getTextureSetIndexForName(const std::string& name) const;
        const TextureSet& getTextureSet(int index) const;
        void removeTextureSet(int index);
        int getTextureSetCount() const;

        std::shared_ptr<Texture> getImage(const std::string& name) const;

        bool isSelected() const;
        void select();
        void resolve();

       protected:
        explicit TexturePack();
        static std::shared_ptr<TexturePack> registerTexturePack(
            const std::shared_ptr<TexturePack> pack);

       private:
        bool selected;
        std::string name;
        std::vector<TextureSet> textureSets;
        std::unordered_map<std::string, std::shared_ptr<Texture> > images;
        static std::vector<std::shared_ptr<TexturePack> > texturePacks;
};
}  // namespace planet
#endif