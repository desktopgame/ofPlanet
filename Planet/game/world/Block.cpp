#include "Block.hpp"

Block::Block(const TexturePack& texturePack, int id)
    : texturePack(texturePack), id(id) {}

void Block::update() {}

void Block::batch(gel::PlaneBatch& batch, int x, int y, int z, Space space) {
        glm::vec4 pos = glm::vec4(x, y, z, 1);
        if (space.posY == nullptr) {
                batch.get(texturePack.posYTex).add(gel::PlaneType::Top, pos);
        }
        if (space.negY == nullptr) {
                batch.get(texturePack.negYTex).add(gel::PlaneType::Bottom, pos);
        }
        if (space.posX == nullptr) {
                batch.get(texturePack.posXTex).add(gel::PlaneType::Right, pos);
        }
        if (space.posZ == nullptr) {
                batch.get(texturePack.posZTex)
                    .add(gel::PlaneType::Forward, pos);
        }
        if (space.negX == nullptr) {
                batch.get(texturePack.negXTex).add(gel::PlaneType::Left, pos);
        }
        if (space.negZ == nullptr) {
                batch.get(texturePack.negZTex)
                    .add(gel::PlaneType::Backward, pos);
        }

        /*
        if (space.negZ == nullptr) {
        }
        if (space.posZ == nullptr) {
        }
        if (space.posX == nullptr) {
        }
        if (space.negX == nullptr) {
        }
        */
        /*
         */
}

TexturePack Block::getTexturePack() const { return texturePack; }

int Block::getID() const { return id; }

int Block::floatToInt(float f) { return (int)std::round(f); }