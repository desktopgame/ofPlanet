#ifndef GAME_WORLD_GEN_PERLINNOISE_HPP
#define GAME_WORLD_GEN_PERLINNOISE_HPP
#include <array>
#include <cmath>
#include "Xorshift.hpp"
// http://edom18.hateblo.jp/entry/2018/10/11/140401
template <int Base>
class PerlinNoise {
       public:
        explicit PerlinNoise(unsigned int seed, float freq);
        float octaveNoise(float x, int octaves, float persistence) const;
        float octaveNoise(float x, float y, int octaves,
                          float persistence) const;
        float octaveNoise(float x, float y, float z, int octaves,
                          float persistence) const;

       private:
        float noise(float x, float y, float z) const;
        static float fade(float t);
        static float lerp(float t, float a, float b);
        static float grad(int hash, float x, float y, float z);
        Xorshift xorshift;
        std::array<int, Base * 2> grid;
        float freq;
};
template <int Base>
PerlinNoise<Base>::PerlinNoise(unsigned int seed, float freq)
    : xorshift(seed), grid(), freq(freq) {
        auto p = std::array<int, Base>();
        for (int i = 0; i < Base; i++) {
                p[i] = (int)std::floor(xorshift.random() * Base);
        }
        for (int i = 0; i < Base * 2; i++) {
                grid[i] = p[i & (Base - 1)];
        }
}
template <int Base>
float PerlinNoise<Base>::octaveNoise(float x, int octaves,
                                     float persistence) const {
        float result = 0;
        float amp = 1.0f;
        float f = freq;
        float maxValue = 0;

        for (int i = 0; i < octaves; i++) {
                result += noise(x * f, 0, 0) * amp;
                f *= 2.0f;
                maxValue += amp;
                amp *= persistence;
        }

        return result / maxValue;
}
template <int Base>
float PerlinNoise<Base>::octaveNoise(float x, float y, int octaves,
                                     float persistence) const {
        float result = 0;
        float amp = 1.0f;
        float f = freq;
        float maxValue = 0;

        for (int i = 0; i < octaves; i++) {
                result += noise(x * f, y * f, 0) * amp;
                f *= 2.0f;
                maxValue += amp;
                amp *= persistence;
        }

        return result / maxValue;
}
template <int Base>
float PerlinNoise<Base>::octaveNoise(float x, float y, float z, int octaves,
                                     float persistence) const {
        float result = 0;
        float amp = 1.0f;
        float f = freq;
        float maxValue = 0;

        for (int i = 0; i < octaves; i++) {
                result += noise(x * f, y * f, z * f) * amp;
                f *= 2.0f;
                maxValue += amp;
                amp *= persistence;
        }

        return result / maxValue;
}
template <int Base>
float PerlinNoise<Base>::noise(float x, float y, float z) const {
        // Repeat while 0 - 255
        int X = (int)std::floor(x) & (Base - 1);
        int Y = (int)std::floor(y) & (Base - 1);
        int Z = (int)std::floor(z) & (Base - 1);

        // trim integer
        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        float u = fade(x);
        float v = fade(y);
        float w = fade(z);

        std::array<int, Base* 2> p = grid;

        //#region## #calulate hashes from array of p## #
        int A, B, AA, AB, BA, BB, AAA, ABA, AAB, ABB, BAA, BBA, BAB, BBB;

        A = p[X + 0] + Y;
        AA = p[A] + Z;
        AB = p[A + 1] + Z;
        B = p[X + 1] + Y;
        BA = p[B] + Z;
        BB = p[B + 1] + Z;

        AAA = p[AA + 0];
        ABA = p[BA + 0];
        AAB = p[AB + 0];
        ABB = p[BB + 0];
        BAA = p[AA + 1];
        BBA = p[BA + 1];
        BAB = p[AB + 1];
        BBB = p[BB + 1];
        //#endregion## #calulate hashes from array of p## #

        float a = grad(AAA, x + 0, y + 0, z + 0);
        float b = grad(ABA, x - 1, y + 0, z + 0);
        float c = grad(AAB, x + 0, y - 1, z + 0);
        float d = grad(ABB, x - 1, y - 1, z + 0);
        float e = grad(BAA, x + 0, y + 0, z - 1);
        float f = grad(BBA, x - 1, y + 0, z - 1);
        float g = grad(BAB, x + 0, y - 1, z - 1);
        float h = grad(BBB, x - 1, y - 1, z - 1);

        return lerp(w, lerp(v, lerp(u, a, b), lerp(u, c, d)),
                    lerp(v, lerp(u, e, f), lerp(u, g, h)));
}
template <int Base>
float PerlinNoise<Base>::fade(float t) {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}
template <int Base>
float PerlinNoise<Base>::lerp(float t, float a, float b) {
        return a + t * (b - a);
}
template <int Base>
float PerlinNoise<Base>::grad(int hash, float x, float y, float z) {
        int h = hash & 15;
        float u = (h < 8) ? x : y;
        float v = (h < 4) ? y : (h == 12 || h == 14) ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
#endif