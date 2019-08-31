#ifndef GAME_WORLD_BIOME_BASICBIOME_HPP
#define GAME_WORLD_BIOME_BASICBIOME_HPP
#include "../../../gel/gel.hpp"
#include "Biome.hpp"
class BasicBiome : public Biome {
       public:
        explicit BasicBiome();
        virtual ~BasicBiome();
        void generate(World& world, gel::Random& random) override;

       protected:
        virtual float fixHeight(float y) const;
        virtual void onGenerateTerrain(World& world, gel::Random& random, int x,
                                       int y, int z);
        virtual void onGenerateWater(World& world, gel::Random& random, int x,
                                     int y, int z);
        virtual void onGenerateStructures(World& world, gel::Random& random);
        virtual void generateStructure(World& world, gel::Random& random,
                                       MultiBlock mb, glm::ivec3 intervalMin,
                                       glm::ivec3 intervalMax, int testCount,
                                       int genLimit);
        virtual std::shared_ptr<Block> getDirtBlock(World& world, int x, int y,
                                                    int z) const;
        virtual std::shared_ptr<Block> getStoneBlock(World& world, int x, int y,
                                                     int z) const;
        virtual std::shared_ptr<Block> getWaterBlock(World& world, int x, int y,
                                                     int z) const;
        ;
        std::unordered_map<glm::ivec2, int, gel::Vec2HashFunc,
                           gel::Vec2HashFunc>
            heightMap;
};
#endif