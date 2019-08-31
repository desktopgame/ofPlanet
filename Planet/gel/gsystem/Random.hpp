#ifndef GEL_GSYSTEM_RANDOM_HPP
#define GEL_GSYSTEM_RANDOM_HPP
#include <random>
namespace gel {
class Random {
       public:
        explicit Random();
        int generate(int min, int max);
        float generate(float min, float max);

       private:
        std::random_device device;
        std::mt19937 engine;
};
}  // namespace gel
#endif