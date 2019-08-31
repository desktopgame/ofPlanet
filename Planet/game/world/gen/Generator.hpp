#ifndef GAME_WORLD_GEN_GENERATOR_HPP
#define GAME_WORLD_GEN_GENERATOR_HPP
#include <algorithm>
#include <vector>
#include "../World.hpp"
#include "Cell.hpp"
#include "PerlinNoise.hpp"
template <int XZSize>
class Generator {
       public:
        explicit Generator();
        std::vector<Cell> generate(unsigned int seed);

        float freq;
        int octaves;
        float persistence;
};
template <int XZSize>
Generator<XZSize>::Generator() : freq(32), persistence(0.5f), octaves(5) {}

template <int XZSize>
std::vector<Cell> Generator<XZSize>::generate(unsigned int seed) {
        PerlinNoise<(XZSize / 2)> noise(seed, freq);
        std::vector<Cell> cells;
        float fx = 1.0f / (float)XZSize;
        float fy = 1.0f / (float)XZSize;
        for (int i = 0; i < XZSize * XZSize; i++) {
                int x = i % XZSize;
                int y = i / XZSize;
                float n =
                    noise.octaveNoise(x * fx, y * fy, octaves, persistence);
                cells.push_back(Cell(x, y, n));
        }
        return cells;
}
#endif