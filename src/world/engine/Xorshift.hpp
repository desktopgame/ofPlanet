#ifndef WORLD_GEN_HPP
#define WORLD_GEN_HPP
#include <array>
namespace planet {
// http://edom18.hateblo.jp/entry/2018/10/11/140401
class Xorshift {
       public:
        explicit Xorshift(unsigned int seed);
        float random();

       private:
        std::array<unsigned int, 4> vec;
};
}  // namespace planet
#endif