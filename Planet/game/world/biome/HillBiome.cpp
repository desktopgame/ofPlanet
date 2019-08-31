#include "HillBiome.hpp"

void HillBiome::onGenerateStructures(World& world, gel::Random& random) {
        MultiBlock woord = MultiBlock::fromFile("./woord.multiblock");
        generateStructure(world, random, woord, glm::ivec3(3, 0, 12),
                          glm::ivec3(3, 0, 12), 100, 20);
}
