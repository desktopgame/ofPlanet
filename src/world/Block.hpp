#pragma once
#ifndef WORLD_BLOCK_HPP
#define WORLD_BLOCK_HPP
#include <memory>
#include <unordered_map>

#include "TextureSet.hpp"
namespace planet {

class World;
class BlockRenderer;
class Block {
       public:
        explicit Block(const std::string& name,
                       const std::string& textureReference, int id);
        virtual ~Block() = default;

        void batch(std::shared_ptr<World> world,
                           BlockRenderer& renderer, int x, int y,
                           int z);

        TextureSet getTextureSet() const;

        void setTextureSetIndex(int textureSetIndex);
        int getTextureSetIndex() const;

        std::string getTextureReference() const;
        std::string getName() const;
        int getID() const;

       protected:
       private:
        std::string textureReference;
        std::string name;
        int id;
        int textureSetIndex;
};
}  // namespace planet
#endif