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
      waterBlock("Water") {}

BasicBiome::~BasicBiome() {}

void BasicBiome::onGUI() {
        ImGui::InputText("TopBlock", topBlock, 255);
        ImGui::InputText("FillBlock", fillBlock, 255);
        ImGui::InputText("FillHardBlock", fillHardBlock, 255);
        ImGui::InputText("WaterBlock", waterBlock, 255);
        ImGui::Checkbox("GenerateCave", &generateCave);
}

void BasicBiome::generate(BlockTable& blockTable) {
        std::random_device seed_gen;
        const int XSIZE = blockTable.getXSize();
        const int YSIZE = blockTable.getYSize();
        const int ZSIZE = blockTable.getZSize();
        const int YSIZE_H = YSIZE / 2;
        const int XZ = (XSIZE + ZSIZE) / 2;
        onBeginGenerate(blockTable);
        if (!this->isUseCallbacks()) {
                onEndGenerate(blockTable);
                return;
        }
        Generator gen(XSIZE, YSIZE, ZSIZE);
        // world.clear();
        heightMap.clear();
        // generate terrain
        auto terrain = gen.generate(seed_gen());
        blockTable.setTerrain(terrain);
        for (int i = 0; i < terrain.getCellCount(); i++) {
                Cell cellSrc = terrain.getCellAt(i);
                Cell cell =
                    Cell(cellSrc.x, cellSrc.z, onFixHeight(cellSrc.noise));
                int y = YSIZE_H + World::floatToInt(cell.noise * (YSIZE_H - 1));
                y = std::min(YSIZE - 1, y);
                heightMap[glm::ivec2(cell.x, cell.z)] = y;
                onGenerateTerrain(blockTable, cell.x, y, cell.z);
        }
        // generate water
        for (auto p : heightMap) {
                auto xz = p.first;
                auto y = p.second;
                int baseY = YSIZE_H - 5;
                if (y > baseY) {
                        continue;
                }
                for (int i = y; i < baseY; i++) {
                        onGenerateWater(blockTable, xz.x, i, xz.y);
                }
        }
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
        onEndGenerate(blockTable);
}

bool BasicBiome::isUseCallbacks() { return true; }

// protected
float BasicBiome::onFixHeight(float y) { return y; }

void BasicBiome::onBeginGenerate(BlockTable& blockTable) {}

void BasicBiome::onEndGenerate(BlockTable& blockTable) {}

void BasicBiome::onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                   int z) {
        blockTable.set(x, y, z, createTopBlock(blockTable, x, y, z));
        int startY = y;
        while (y-- > 0) {
                blockTable.set(x, y, z,
                               createFillBlock(blockTable, startY, x, y, z));
        }
}

void BasicBiome::onGenerateWater(BlockTable& blockTable, int x, int y, int z) {
        blockTable.set(x, y, z, createWaterBlock(blockTable, x, y, z));
}

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
                                   int genLimit) {
        int success = 0;
        for (int n = 0; n < testCount; n++) {
                int x = static_cast<int>(
                    random.generate(0, blockTable.getXSize() - 1));
                int z = static_cast<int>(
                    random.generate(0, blockTable.getZSize() - 1));
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
                        blockTable.expand(x + 1, y + 1, z + 1, mb);
                }
                if (success >= genLimit) {
                        break;
                }
        }
}

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
