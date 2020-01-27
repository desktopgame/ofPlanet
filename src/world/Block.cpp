#include "Block.hpp"

#include "../common/GLM.hpp"
#include "Texture.hpp"
#include "BlockRenderer.hpp"
#include "TexturePack.hpp"
#include "World.hpp"
namespace planet {
Block::Block(const std::string& name, const std::string& textureReference,
             int id)
    : name(name), textureReference(textureReference), id(id) {}
void Block::batch(std::shared_ptr<World> world, BlockRenderer& renderer, int x,
                  int y, int z) {
        TextureSet set = getTextureSet();
        const float size = 2.0f;
        //
        if (world->isEmpty(x - 1, y, z)) {
                renderer.putLeft(set.getLeftImage()->getName(), x, y, z);
        }
        //
        if (world->isEmpty(x + 1, y, z)) {
                renderer.putRight(set.getRightImage()->getName(), x, y, z);
        }
        //
        if (world->isEmpty(x, y, z - 1)) {
                renderer.putBack(set.getBackImage()->getName(), x, y, z);
        }
        //
        if (world->isEmpty(x, y, z + 1)) {
                renderer.putFront(set.getFrontImage()->getName(), x, y, z);
        }
        //
        if (world->isEmpty(x, y + 1, z)) {
                renderer.putTop(set.getTopImage()->getName(), x, y, z);
        }
        //
        if (world->isEmpty(x, y - 1, z)) {
                renderer.putBottom(set.getBottomImage()->getName(), x, y, z);
        }
}

TextureSet Block::getTextureSet() const {
        return TexturePack::getCurrent()->getTextureSet(textureSetIndex);
}

void Block::setTextureSetIndex(int textureSetIndex) {
        this->textureSetIndex = textureSetIndex;
}

int Block::getTextureSetIndex() const { return textureSetIndex; }

std::string Block::getTextureReference() const {
        return this->textureReference;
}

std::string Block::getName() const { return name; }

int Block::getID() const { return id; }

}  // namespace planet