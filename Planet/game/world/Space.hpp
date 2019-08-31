#ifndef GAME_WORLD_SPACE_HPP
#define GAME_WORLD_SPACE_HPP
#include <memory>
#include "../../gel/gel.hpp"
class Block;
class Space {
       public:
        explicit Space();
        bool isHoled() const;
        bool isFilled() const;
        std::shared_ptr<Block> posX;
        std::shared_ptr<Block> negX;
        std::shared_ptr<Block> posY;
        std::shared_ptr<Block> negY;
        std::shared_ptr<Block> posZ;
        std::shared_ptr<Block> negZ;
};
#endif