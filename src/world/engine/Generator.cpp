#include "Generator.hpp"
#include <iostream>
namespace planet {

Generator::Generator(const glm::ivec3& size)
    : size(size), freq(32), persistence(0.5f), octaves(5) {}

Generator::Generator(int xSize, int ySize, int zSize) : Generator(glm::ivec3(xSize, ySize, zSize)) {
}

Terrain Generator::generate(unsigned int seed) {
		Terrain ter;
		int xzSize = (size.x + size.z) / 2;
        PerlinNoise noise(xzSize / 2, seed, freq);
        std::vector<Cell> cells;
        float fx = 1.0f / (float)xzSize;
        float fy = 1.0f / (float)xzSize;
		float fz = 1.0f / (float)size.y;
        for (int i = 0; i < xzSize * xzSize; i++) {
                int x = i % xzSize;
                int y = i / xzSize;
                float n = noise.octaveNoise(x * fx, y * fy, octaves, persistence);
				for (int z = 0; z < size.y; z++) {
					//’n•\‚ÍÁ‚³‚È‚¢
					if (z > this->size.y / 2) {
						continue;
					}
					float thickP = noise3D(noise, x, z, y);
					ter.addPocket(Pocket(x, z, y, thickP));
				}
                ter.addCell(Cell(x, y, n));
        }
		return ter;
}

float Generator::noise3D(PerlinNoise& noise, int x, int y, int z) {
	int xzSize = (size.x + size.z) / 2;
	float fxm = 1.0f / (float)xzSize;
	float fym = 1.0f / (float)xzSize;
	float fzm = 1.0f / (float)size.y;
	float fx = static_cast<float>(x) * fxm;
	float fy = static_cast<float>(y) * fzm;
	float fz = static_cast<float>(z) * fym;
	return noise.octaveNoise(fx, fy, fz, octaves, persistence);
}

}  // namespace planet
