#include "Random.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
namespace gel {
Random::Random() : device(), engine(device()) {}

int Random::generate(int min, int max) {
        std::uniform_int_distribution<int> d(min, max);
        return d(engine);
}
float Random::generate(float min, float max) {
        std::uniform_real_distribution<float> d(min, max);
        return d(engine);
}
}  // namespace gel