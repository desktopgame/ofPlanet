#ifndef GEL_GSYSTEM_DURATION_HPP
#define GEL_GSYSTEM_DURATION_HPP
#include "Timer.hpp"
namespace gel {
class Duration {
       public:
        explicit Duration(float length);
        void update();
        float progress() const;

       private:
        Timer timer;
        bool increase;
};
}  // namespace gel
#endif