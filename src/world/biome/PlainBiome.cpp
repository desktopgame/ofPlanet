#include "PlainBiome.hpp"
namespace planet {

PlainBiome::PlainBiome() {}

PlainBiome::~PlainBiome() {}

float PlainBiome::onFixHeight(float y) {
        if (y < 0) {
                return 0;
        }
        return y;
}

void PlainBiome::onGenerateWater(BlockTable& blockTable, int x, int y, int z) {
        // no generate
}

void PlainBiome::onGenerateStructures(BlockTable& blockTable) {
	/*
        MultiBlock woord =
            MultiBlock::fromFile("./data/block/woord.multiblock");
        generateStructure(blockTable, woord, glm::ivec3(3, 0, 12),
                          glm::ivec3(3, 0, 12), 200, 20);
						  */

        // MultiBlock room = MultiBlock::fromFile("./room.multiblock");
        // generateStructure(world, random, room, glm::ivec3(19, 0, 19),
        //                  glm::ivec3(19, 0, 19), 1000, 2);
}
}  // namespace planet
