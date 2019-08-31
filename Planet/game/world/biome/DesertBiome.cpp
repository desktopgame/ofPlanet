#include "DesertBiome.hpp"

DesertBiome::DesertBiome() {}

DesertBiome::~DesertBiome() {}
float DesertBiome::fixHeight(float y) const {
        if (y < 0) {
                return 0;
        }
        return y;
}

void DesertBiome::onGenerateWater(World& world, gel::Random& random, int x,
                                  int y, int z) {
        // no generate
}

void DesertBiome::onGenerateStructures(World& world, gel::Random& random) {
        MultiBlock woord = MultiBlock::fromFile("./pyramid.multiblock");
        generateStructure(world, random, woord, glm::ivec3(24, 0, 24),
                          glm::ivec3(24, 0, 24), 100, 4);
}

std::shared_ptr<Block> DesertBiome::getDirtBlock(World& world, int x, int y,
                                                 int z) const {
        return BlockRegistry::getInstance().getBlock(2);
}

std::shared_ptr<Block> DesertBiome::getStoneBlock(World& world, int x, int y,
                                                  int z) const {
        return BlockRegistry::getInstance().getBlock(2);
}
