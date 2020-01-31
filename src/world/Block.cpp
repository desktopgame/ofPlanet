#include "Block.hpp"

#include "../common/GLM.hpp"
#include "Texture.hpp"
#include "BlockRenderer.hpp"
#include "TexturePack.hpp"
#include "World.hpp"
namespace planet {
Block::Block(BlockShape shape, const std::string& name, const std::string& textureReference,
             int id)
    : shape(shape), name(name), textureReference(textureReference), id(id) {}
void Block::batch(std::shared_ptr<World> world, BlockRenderer& renderer, int x,
                  int y, int z) {
        TextureSet set = getTextureSet();
		std::reference_wrapper<GraphicsRenderer> target = renderer.getCubeRenderer();
		if (this->shape == BlockShape::HorizontalSlab) {
			target = renderer.getHorizontalSlabRenderer();
		} else if (this->shape == BlockShape::VerticalSlab) {
			target = renderer.getVerticalSlabRenderer();
		}
        if (!world->isFilled(x - 1, y, z)) {
				target.get().putLeft(set.getLeftImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x + 1, y, z)) {
				target.get().putRight(set.getRightImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x, y, z - 1)) {
				target.get().putBack(set.getBackImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x, y, z + 1)) {
				target.get().putFront(set.getFrontImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x, y + 1, z)) {
				target.get().putTop(set.getTopImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x, y - 1, z)) {
				target.get().putBottom(set.getBottomImage()->getName(), x, y, z);
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

glm::vec3 Block::getSize() const {
	if (this->shape == BlockShape::Block) {
		return glm::vec3(2, 2, 2);
	} else if (this->shape == BlockShape::HorizontalSlab) {
		return glm::vec3(2, 2, 1);
	} else if (this->shape == BlockShape::VerticalSlab) {
		return glm::vec3(2, 1, 2);
	}
	return glm::vec3(2, 2, 2);
}

BlockShape Block::getShape() const {
	return shape;
}

int Block::getID() const { return id; }

BlockShape stringToBlockShape(const std::string & str) {
	if (str == "Block" || str == "BLOCK") {
		return BlockShape::Block;
	}
	if (str == "HorizontalSlab" || str == "HORIZONTALSLAB") {
		return BlockShape::HorizontalSlab;
	}
	if (str == "VerticalSlab" || str == "VERTICALSLAB") {
		return BlockShape::VerticalSlab;
	}
	return BlockShape::Block;
}
}  // namespace planet