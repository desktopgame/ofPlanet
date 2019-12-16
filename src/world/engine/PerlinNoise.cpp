#include "PerlinNoise.hpp"
namespace planet {
PerlinNoise::PerlinNoise(int base, unsigned int seed, float freq)
    : base(base), xorshift(seed), grid(), freq(freq) {
        auto p = std::vector<int>();
        for (int i = 0; i < base; i++) {
                p.emplace_back((int)std::floor(xorshift.random() * base));
        }
        for (int i = 0; i < base * 2; i++) {
                grid.emplace_back(p[i & (base - 1)]);
        }
}

float PerlinNoise::octaveNoise(float x, int octaves, float persistence) const {
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

float PerlinNoise::octaveNoise(float x, float y, int octaves,
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

float PerlinNoise::octaveNoise(float x, float y, float z, int octaves,
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

float PerlinNoise::noise(float x, float y, float z) const {
        // Repeat while 0 - 255
        int X = (int)std::floor(x) & (base - 1);
        int Y = (int)std::floor(y) & (base - 1);
        int Z = (int)std::floor(z) & (base - 1);

        // trim integer
        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        float u = fade(x);
        float v = fade(y);
        float w = fade(z);

        std::vector<int> p = grid;

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

float PerlinNoise::fade(float t) {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::lerp(float t, float a, float b) { return a + t * (b - a); }

float PerlinNoise::grad(int hash, float x, float y, float z) {
        int h = hash & 15;
        float u = (h < 8) ? x : y;
        float v = (h < 4) ? y : (h == 12 || h == 14) ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
}  // namespace planet
