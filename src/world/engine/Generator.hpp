#ifndef GAME_WORLD_GEN_GENERATOR_HPP
#define GAME_WORLD_GEN_GENERATOR_HPP
#include <algorithm>
#include <vector>

#include "../World.hpp"
#include "Terrain.hpp"
#include "PerlinNoise.hpp"
#include "../PerlinNoise/PerlinNoise.hpp"
namespace planet {
class Generator {
       public:
        explicit Generator(const glm::ivec3& size);
		explicit Generator(int xSize, int ySize, int zSize);
        Terrain generate(unsigned int seed);

        float freq;
        int octaves;
        float persistence;
		glm::ivec3 size;
private:
	siv::PerlinNoise sivPerlin;
	float noise3D(PerlinNoise& noise, int x, int y, int z);
};
}  // namespace planet
#endif