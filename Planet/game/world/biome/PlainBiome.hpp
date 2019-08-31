#ifndef GAME_WORLD_BIOME_PLAINBIOME_HPP
#define GAME_WORLD_BIOME_PLAINBIOME_HPP
#include "BasicBiome.hpp"

class PlainBiome : public BasicBiome {
       public:
        explicit PlainBiome();
        virtual ~PlainBiome();

       protected:
        float fixHeight(float y) const override;
        void onGenerateWater(World& world, gel::Random& random, int x, int y,
                             int z) override;
        void onGenerateStructures(World& world, gel::Random& random) override;
};
#endif