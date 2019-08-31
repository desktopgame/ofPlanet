#include "BlockRegistry.hpp"
BlockRegistry::BlockRegistry() : blocks() {}

BlockRegistry& BlockRegistry::getInstance() {
        static BlockRegistry instance;
        return instance;
}

void BlockRegistry::addBlock(const TexturePack pack) {
        blocks.push_back(std::make_shared<Block>(pack, blocks.size()));
}

std::shared_ptr<Block> BlockRegistry::getBlock(int id) const {
        assert(blocks.size() > 0);
        return blocks[id];
}

int BlockRegistry::getBlockCount() const { return blocks.size(); }
