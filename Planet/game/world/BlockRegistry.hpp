#ifndef GAME_WORLD_BLOCKREGISTRY_HPP
#define GAME_WORLD_BLOCKREGISTRY_HPP
#include "../../gel/gel.hpp"
#include "Block.hpp"
class BlockRegistry {
       public:
        BlockRegistry(const BlockRegistry&) = delete;
        BlockRegistry& operator=(const BlockRegistry&) = delete;
        BlockRegistry(BlockRegistry&&) = delete;
        BlockRegistry& operator=(BlockRegistry&&) = delete;

        static BlockRegistry& getInstance();
        void addBlock(const TexturePack pack);
        std::shared_ptr<Block> getBlock(int id) const;
        int getBlockCount() const;

       private:
        BlockRegistry();
        ~BlockRegistry() = default;
        std::vector<std::shared_ptr<Block> > blocks;
};
#endif