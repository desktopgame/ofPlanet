#include "Xorshift.hpp"
#include <algorithm>
Xorshift::Xorshift(unsigned int seed) {
        std::fill(vec.begin(), vec.end(), 0);
        for (int i = 1; i < 4; i++) {
                seed = 1812433253 * (seed ^ (seed >> 30)) + i;
                vec[i - 1] = seed;
        }
}
float Xorshift::random() {
        unsigned int t = vec[0];
        unsigned int w = vec[3];

        vec[0] = vec[1];
        vec[1] = vec[2];
        vec[2] = w;

        t ^= t << 11;
        t ^= t >> 8;
        w ^= w >> 19;
        w ^= t;

        vec[3] = w;

        return w * 2.3283064365386963e-10f;
}