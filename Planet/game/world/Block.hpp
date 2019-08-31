#ifndef GAME_WORLD_BLOCK_HPP
#define GAME_WORLD_BLOCK_HPP
#include <memory>
#include <string>
#include "../../gel/gel.hpp"
#include "Space.hpp"
#include "TexturePack.hpp"
class Block {
       public:
        explicit Block(const TexturePack& texturePack, int id);
        virtual ~Block() = default;
        virtual void update();
        virtual void batch(gel::PlaneBatch& batch, int x, int y, int z,
                           Space space);
        TexturePack getTexturePack() const;
        int getID() const;
        static int floatToInt(float f);

       private:
        TexturePack texturePack;
        int id;
};
#endif