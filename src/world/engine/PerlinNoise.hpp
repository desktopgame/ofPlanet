#pragma once
#ifndef WORLD_GEN_PERLINNOISE_HPP
#define WORLD_GEN_PERLINNOISE_HPP
#include <array>
#include <cmath>
#include <vector>

#include "Xorshift.hpp"
namespace planet {
// http://edom18.hateblo.jp/entry/2018/10/11/140401
class PerlinNoise {
       public:
        explicit PerlinNoise(int base, unsigned int seed, float freq);
        float octaveNoise(float x, int octaves, float persistence) const;
        float octaveNoise(float x, float y, int octaves,
                          float persistence) const;
        float octaveNoise(float x, float y, float z, int octaves,
                          float persistence) const;

        float noise(float x, float y, float z) const;
       private:
        static float fade(float t);
        static float lerp(float t, float a, float b);
        static float grad(int hash, float x, float y, float z);
        Xorshift xorshift;
        std::vector<int> grid;
        float freq;
        int base;
};
}  // namespace planet
#endif