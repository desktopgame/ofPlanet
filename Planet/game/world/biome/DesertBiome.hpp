#ifndef GAME_WORLD_BIOME_DESERTBIOME_HPP
#define GAME_WORLD_BIOME_DESERTBIOME_HPP
#include "BasicBiome.hpp"
class DesertBiome : public BasicBiome {
       public:
        explicit DesertBiome();
        virtual ~DesertBiome();

       protected:
        float fixHeight(float y) const override;
        void onGenerateWater(World& world, gel::Random& random, int x, int y,
                             int z) override;
        void onGenerateStructures(World& world, gel::Random& random) override;
        std::shared_ptr<Block> getDirtBlock(World& world, int x, int y,
                                            int z) const override;
        std::shared_ptr<Block> getStoneBlock(World& world, int x, int y,
                                             int z) const override;
};
#endif