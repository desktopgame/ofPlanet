#pragma once
#ifndef WORLD_BIOME_PLAINBIOME_HPP
#define WORLD_BIOME_PLAINBIOME_HPP
#include "BasicBiome.hpp"
namespace planet {

class PlainBiome : public BasicBiome {
       public:
        explicit PlainBiome();
        virtual ~PlainBiome();

       protected:
        float onFixHeight(float y) override;
        void onGenerateWater(BlockTable& blockTable, int x, int y,
                             int z) override;
        void onGenerateStructures(BlockTable& blockTable) override;
};
}  // namespace planet
#endif