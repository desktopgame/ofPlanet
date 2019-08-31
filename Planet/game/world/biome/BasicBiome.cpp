#include "BasicBiome.hpp"
#include "../../def.hpp"
#include "../gen/Generator.hpp"

BasicBiome::BasicBiome() : heightMap() {}

BasicBiome::~BasicBiome() {}

void BasicBiome::generate(World& world, gel::Random& random) {
        Generator<(WSIZE_X + WSIZE_Z) / 2> gen;
        gen.freq = 4.0f;
        world.clear();
        heightMap.clear();
        // generate terrain
        auto cells = gen.generate(random.generate(0, 10000));
        auto iter = cells.begin();
        while (iter != cells.end()) {
                auto& cell = *iter;
                cell.noise = fixHeight(cell.noise);
                int y = WSIZE_HALF_Y +
                        Block::floatToInt(cell.noise * (WSIZE_HALF_Y - 1));
                heightMap[glm::ivec2(cell.x, cell.z)] = y;
                onGenerateTerrain(world, random, cell.x, y, cell.z);
                ++iter;
        }
        // generate water
        for (auto p : heightMap) {
                auto xz = p.first;
                auto y = p.second;
                int baseY = WSIZE_HALF_Y - 5;
                if (y > baseY) {
                        continue;
                }
                for (int i = y; i < baseY; i++) {
                        onGenerateWater(world, random, xz.x, i, xz.y);
                }
        }
        // generate structure
        onGenerateStructures(world, random);
}

// protected
float BasicBiome::fixHeight(float y) const { return y; }

void BasicBiome::onGenerateTerrain(World& world, gel::Random& random, int x,
                                   int y, int z) {
        world.setBlockAt(x, y, z, getDirtBlock(world, x, y, z));
        while (y-- > 0) {
                world.setBlockAt(x, y, z, getStoneBlock(world, x, y, z));
        }
}

void BasicBiome::onGenerateWater(World& world, gel::Random& random, int x,
                                 int y, int z) {
        world.setBlockAt(x, y, z, getWaterBlock(world, x, y, z));
}

void BasicBiome::onGenerateStructures(World& world, gel::Random& random) {}

void BasicBiome::generateStructure(World& world, gel::Random& random,
                                   MultiBlock mb, glm::ivec3 intervalMin,
                                   glm::ivec3 intervalMax, int testCount,
                                   int genLimit) {
        int success = 0;
        for (int n = 0; n < testCount; n++) {
                int x = random.generate(0, WSIZE_X);
                int z = random.generate(0, WSIZE_X);
                int y = heightMap[glm::ivec2(x, z)];
                bool fail = false;
                for (int l = 0; l < mb.getXSize(); l++) {
                        for (int p = 0; p < mb.getZSize(); p++) {
                                glm::ivec2 key((x + p, z + l));
                                if (!heightMap.count(key) ||
                                    y != heightMap[key]) {
                                        fail = true;
                                        break;
                                }
                        }
                        if (fail) {
                                break;
                        }
                }
                if (!fail) {
                        success++;
                        world.expand(x + 1, y + 1, z + 1, mb);
                }
                if (success >= genLimit) {
                        break;
                }
        }
}

std::shared_ptr<Block> BasicBiome::getDirtBlock(World& world, int x, int y,
                                                int z) const {
        return BlockRegistry::getInstance().getBlock(0);
}

std::shared_ptr<Block> BasicBiome::getStoneBlock(World& world, int x, int y,
                                                 int z) const {
        return BlockRegistry::getInstance().getBlock(1);
}

std::shared_ptr<Block> BasicBiome::getWaterBlock(World& world, int x, int y,
                                                 int z) const {
        return BlockRegistry::getInstance().getBlock(6);
}
