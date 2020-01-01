#include "Generator.hpp"
#include <iostream>
namespace planet {

Generator::Generator(const glm::ivec3& size)
    : size(size), freq((size.x + size.z) / 8), persistence(0.5f), octaves(5), sivPerlin(){}

Generator::Generator(int xSize, int ySize, int zSize) : Generator(glm::ivec3(xSize, ySize, zSize)) {
}

Terrain Generator::generate(unsigned int seed) {
		Terrain ter;
		int xzSize = (size.x + size.z) / 2;
        //PerlinNoise noise((size.x + size.z), seed, freq);
        std::vector<Cell> cells;
		const float frequency = 5.0f;
		const float fx = size.x / frequency;
		const float fy = size.z / frequency;
		const float fz = size.y / frequency;
		for (int i = 0; i < size.x * size.z; i++) {
			int ix = i % xzSize;
			int iy = i / xzSize;
			float noise = static_cast<float>(sivPerlin.octaveNoise(ix / fx, iy / fy, frequency));
			for (int z = 0; z < size.y; z++) {
				if (z > this->size.y / 2) {
					continue;
				}
				float thickP = static_cast<float>(sivPerlin.octaveNoise(ix / fx, iy / fy, z / fz, frequency));
				ter.addPocket(Pocket(ix, z, iy, thickP));
			}
			ter.addCell(Cell(ix, iy, noise));
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
