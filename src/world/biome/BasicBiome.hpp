#pragma once
#ifndef WORLD_BIOME_BASICBIOME_HPP
#define WORLD_BIOME_BASICBIOME_HPP
#include <ofVec3f.h>

#include <unordered_map>

#include "../BlockTable.hpp"
#include "../MultiBlock.hpp"
#include "../common/Random.hpp"
#include "Biome.hpp"
namespace planet {

namespace hidden {
struct Vec2HashFunc {
        size_t operator()(const glm::ivec2& k) const {
                return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
        }

        bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
                return a.x == b.x && a.y == b.y;
        }
};
}  // namespace hidden

class Block;
class BasicBiome : public Biome {
       public:
        explicit BasicBiome();
        virtual ~BasicBiome();
        virtual void onGUI() override;
        void generate(BlockTable& blockTable) override;

       protected:
        virtual bool isUseCallbacks();
        virtual float onFixHeight(float y);
        virtual void onBeginGenerateWorld(BlockTable& blockTable);
        virtual void onEndGenerateWorld(BlockTable& blockTable);

		virtual void onBeginGenerateTerrain();
        virtual void onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                       int z);
		virtual void onEndGenerateTerrain();

        virtual void onGenerateStructures(BlockTable& blockTable);
        virtual void onGenerateCave(BlockTable& blockTable, int x, int y, int z,
                                    float noise);
        virtual void generateStructure(BlockTable& blockTable, MultiBlock mb,
                                       glm::ivec3 intervalMin,
                                       glm::ivec3 intervalMax, int testCount,
                                       int genLimit);
        virtual BlockPrefab createTopBlock(BlockTable& blockTable, int x, int y,
                                           int z) const;
        virtual BlockPrefab createFillBlock(BlockTable& blockTable, int startY,
                                            int x, int y, int z) const;
        virtual BlockPrefab createWaterBlock(BlockTable& blockTable, int x,
                                             int y, int z) const;
        Random random;
        std::unordered_map<glm::ivec2, int, hidden::Vec2HashFunc,
                           hidden::Vec2HashFunc>
            heightMap;
        char topBlock[255];
        char fillBlock[255];
        char fillHardBlock[255];
        char waterBlock[255];
        bool generateCave;
};
}  // namespace planet
#endif