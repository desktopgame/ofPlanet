#ifndef GAME_WORLD_BIOME_HILLBIOME_HPP
#define GAME_WORLD_BIOME_HILLBIOME_HPP
#include "BasicBiome.hpp"
class HillBiome : public BasicBiome {
       public:
        explicit HillBiome() = default;
        virtual ~HillBiome() = default;

       protected:
        void onGenerateStructures(World& world, gel::Random& random) override;
};
#endif