#pragma once
#ifndef WORLD_BIOME_BASICBIOME_HPP
#define WORLD_BIOME_BASICBIOME_HPP
#include <ofVec3f.h>

#include <unordered_map>

#include "../BlockTable.hpp"
#include "../MultiBlock.hpp"
#include "../common/Random.hpp"
#include "../IntVec2Hash.hpp"
#include "../WeightTable.hpp"
#include "Biome.hpp"
namespace planet {
using HeightMap = std::unordered_map<glm::ivec2, int, Vec2HashFunc, Vec2HashFunc>;
using MultiBlockMap = std::unordered_map<std::string, MultiBlock>;
using WeightTableMap = std::unordered_map<std::string, WeightTable>;

class Block;
class BasicBiome : public Biome {
       public:
        explicit BasicBiome();
        virtual ~BasicBiome();
        void generate(BlockTable& blockTable) override;

		void registerStruct(const std::string& name, const MultiBlock& mb);
		void generateStruct(const std::string& name, const glm::ivec3& pos, int addWeight, int limitWeight);
		MultiBlock& getMultiBlock(const std::string& name) const;
		WeightTable& getWeightTable(const std::string& name);

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

private:
	BlockPrefab createTopBlock(BlockTable& blockTable, int x, int y,
		int z) const;
	BlockPrefab createFillBlock(BlockTable& blockTable, int startY,
		int x, int y, int z) const;
	BlockPrefab createWaterBlock(BlockTable& blockTable, int x,
		int y, int z) const;
        Random random;
        char topBlock[255];
        char fillBlock[255];
        char fillHardBlock[255];
        char waterBlock[255];
        bool generateCave;

		glm::ivec3 worldSize;
		std::shared_ptr<HeightMap> heightMap;
		std::shared_ptr<MultiBlockMap> multiBlockMap;
		std::shared_ptr<WeightTableMap> weightTableMap;
protected:

};
}  // namespace planet
#endif