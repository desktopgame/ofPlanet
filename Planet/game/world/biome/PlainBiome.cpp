#include "PlainBiome.hpp"

PlainBiome::PlainBiome() {}

PlainBiome::~PlainBiome() {}

float PlainBiome::fixHeight(float y) const {
        if (y < 0) {
                return 0;
        }
        return y;
}

void PlainBiome::onGenerateWater(World& world, gel::Random& random, int x,
                                 int y, int z) {
        // no generate
}

void PlainBiome::onGenerateStructures(World& world, gel::Random& random) {
        MultiBlock woord = MultiBlock::fromFile("./woord.multiblock");
        generateStructure(world, random, woord, glm::ivec3(3, 0, 12),
                          glm::ivec3(3, 0, 12), 200, 20);

        // MultiBlock room = MultiBlock::fromFile("./room.multiblock");
        // generateStructure(world, random, room, glm::ivec3(19, 0, 19),
        //                  glm::ivec3(19, 0, 19), 1000, 2);
}
