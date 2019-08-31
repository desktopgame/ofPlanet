#ifndef GAME_WORLD_BIOME_BIOME_HPP
#define GAME_WORLD_BIOME_BIOME_HPP
#include <glm/glm.hpp>
#include "../../../gel/gel.hpp"
#include "../World.hpp"
class Biome {
       public:
        explicit Biome() = default;
        virtual ~Biome() = 0;
        virtual void generate(World& world, gel::Random& random) = 0;
};
#endif