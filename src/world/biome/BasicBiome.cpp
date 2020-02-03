#include "BasicBiome.hpp"

#include <ofMath.h>

#include <random>

#include "../Block.hpp"
#include "../BlockPack.hpp"
#include "../World.hpp"
#include "../engine/Generator.hpp"
namespace planet {

BasicBiome::BasicBiome()
    : random(),
      heightMap(),
      topBlock("GrassDirt"),
      fillBlock("Stone"),
      fillHardBlock("HardStone"),
      waterBlock("Water"),
	  multiBlockMap(),
	  weightTableMap() {}

BasicBiome::~BasicBiome() {}

void BasicBiome::generate(BlockTable& blockTable) {
        std::random_device seed_gen;
        const int XSIZE = blockTable.getXSize();
        const int YSIZE = blockTable.getYSize();
        const int ZSIZE = blockTable.getZSize();
        const int YSIZE_H = YSIZE / 2;
        const int XZ = (XSIZE + ZSIZE) / 2;
		this->worldSize = glm::ivec3(XSIZE, YSIZE, ZSIZE);
		this->heightMap = std::make_shared<HeightMap>();
		this->multiBlockMap = std::make_shared<MultiBlockMap>();
		this->weightTableMap = std::make_shared<WeightTableMap>();
        onBeginGenerateWorld(blockTable);
        if (!this->isUseCallbacks()) {
                onEndGenerateWorld(blockTable);
                return;
        }
        Generator gen(XSIZE, YSIZE, ZSIZE);
        // generate terrain
        auto terrain = gen.generate(seed_gen());
        blockTable.setTerrain(terrain);
        onBeginGenerateTerrain();
        for (int i = 0; i < terrain.getCellCount(); i++) {
                Cell cellSrc = terrain.getCellAt(i);
                Cell cell =
                    Cell(cellSrc.x, cellSrc.z, onFixHeight(cellSrc.noise));
                int y = YSIZE_H + World::floatToInt(cell.noise * (YSIZE_H - 1));
                y = std::min(YSIZE - 1, y);
				heightMap->insert_or_assign(glm::ivec2(cell.x, cell.z), y);
                onGenerateTerrain(blockTable, cell.x, y, cell.z);
        }
        onEndGenerateTerrain();
        // generate structure
        onGenerateStructures(blockTable);
        // generate cave
        if (this->generateCave) {
                for (int i = 0; i < terrain.getPocketCount(); i++) {
                        Pocket pocket = terrain.getPocketAt(i);
                        onGenerateCave(blockTable, pocket.x, pocket.y, pocket.z,
                                       pocket.zNoise);
                }
        }
        onEndGenerateWorld(blockTable);
}

WeightTable& BasicBiome::getWeightTable(const std::string & name) {
	if (!this->weightTableMap->count(name)) {
		WeightTable wt(this->worldSize.x, this->worldSize.y, this->worldSize.z);
		this->weightTableMap->insert_or_assign(name, wt);
	}
	return weightTableMap->at(name);
}

bool BasicBiome::isUseCallbacks() { return true; }

// protected
float BasicBiome::onFixHeight(float y) { return y; }

void BasicBiome::onBeginGenerateWorld(BlockTable& blockTable) {}

void BasicBiome::onEndGenerateWorld(BlockTable& blockTable) {}

void BasicBiome::onBeginGenerateTerrain() {}

void BasicBiome::onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                   int z) {
        blockTable.set(x, y, z, createTopBlock(blockTable, x, y, z));
        int startY = y;
        while (y-- > 0) {
                blockTable.set(x, y, z,
                               createFillBlock(blockTable, startY, x, y, z));
        }
}

void BasicBiome::onEndGenerateTerrain() {}

void BasicBiome::onGenerateStructures(BlockTable& blockTable) {}

void BasicBiome::onGenerateCave(BlockTable& blockTable, int x, int y, int z,
                                float noise) {
        if (noise > 0.1f) {
                BlockPrefab pref(-1, false);
                blockTable.set(x, y, z, pref);
        }
}

void BasicBiome::generateStructure(BlockTable& blockTable, MultiBlock mb,
                                   glm::ivec3 intervalMin,
                                   glm::ivec3 intervalMax, int testCount,
                                   int genLimit) {}

void BasicBiome::registerStruct(const std::string & name, const MultiBlock & mb) {
	this->multiBlockMap->insert_or_assign(name, mb);
}

void BasicBiome::generateStruct(const std::string & name, const glm::ivec3 & pos, int addWeight, int limitWeight) {
}

MultiBlock & BasicBiome::getMultiBlock(const std::string & name) const {
	return this->multiBlockMap->at(name);
}

// private
BlockPrefab BasicBiome::createTopBlock(BlockTable& blockTable, int x, int y,
                                       int z) const {
        return BlockPrefab(
            BlockPack::getCurrent()->getBlockIndexForName(topBlock), false);
}

BlockPrefab BasicBiome::createFillBlock(BlockTable& blockTable, int startY,
                                        int x, int y, int z) const {
        if (y < (startY - 10)) {
                return BlockPrefab(
                    BlockPack::getCurrent()->getBlockIndexForName(
                        fillHardBlock),
                    false);
        } else {
                return BlockPrefab(
                    BlockPack::getCurrent()->getBlockIndexForName(fillBlock),
                    false);
        }
}

BlockPrefab BasicBiome::createWaterBlock(BlockTable& blockTable, int x, int y,
                                         int z) const {
        return BlockPrefab(
            BlockPack::getCurrent()->getBlockIndexForName(waterBlock), false);
}
}  // namespace planet
