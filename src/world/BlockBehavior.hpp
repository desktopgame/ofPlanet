#pragma once
#ifndef WORLD_BLOCKBEHAVIOR_HPP
#define WORLD_BLOCKBEHAVIOR_HPP
#include <memory>

#include "BlockColliderType.hpp"
namespace planet {

class World;
class BlockRenderer;
class BlockBehavior : public std::enable_shared_from_this<BlockBehavior> {
       public:
        BlockBehavior() = default;
        virtual ~BlockBehavior() = default;
        virtual void update(std::shared_ptr<World> world, int x, int y,
                            int z) = 0;
        virtual void batch(std::shared_ptr<World> world,
                           BlockRenderer& renderer, int x, int y, int z) = 0;
        virtual BlockColliderType getColliderType(std::shared_ptr<World> world,
                                                  int x, int y,
                                                  int z) const = 0;

       private:
};
}  // namespace planet
#endif