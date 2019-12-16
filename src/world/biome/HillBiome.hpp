#pragma once
#ifndef WORLD_BIOME_HILLBIOME_HPP
#define WORLD_BIOME_HILLBIOME_HPP
#include "BasicBiome.hpp"
namespace planet {
class HillBiome : public BasicBiome {
       public:
        explicit HillBiome() = default;
        virtual ~HillBiome() = default;

       protected:
        void onGenerateStructures(BlockTable& blockTable) override;
};
}  // namespace planet
#endif