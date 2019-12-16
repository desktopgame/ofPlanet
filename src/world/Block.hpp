#pragma once
#ifndef WORLD_BLOCK_HPP
#define WORLD_BLOCK_HPP
#include <memory>
#include <unordered_map>

#include "BlockBehavior.hpp"
#include "TextureSet.hpp"
namespace planet {

class BlockRenderer;
class Block : public BlockBehavior {
       public:
		explicit Block(const std::string& name, const std::string& textureReference, int id);
        virtual ~Block() = default;

        virtual std::shared_ptr<BlockBehavior> newBehavior();
        virtual void update(std::shared_ptr<World> world, int x, int y,
                            int z) override;
        virtual void batch(std::shared_ptr<World> world,
                           BlockRenderer& renderer, int x, int y,
                           int z) override;
        virtual BlockColliderType getColliderType(std::shared_ptr<World> world,
                                                  int x, int y,
                                                  int z) const override;

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