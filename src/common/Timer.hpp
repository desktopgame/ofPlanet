#pragma once
#ifndef COMMON_TIMER_HPP
#define COMMON_TIMER_HPP
namespace planet {
class Timer {
       public:
        explicit Timer(float seconds);

        void update();
        void reset();
        float parcent() const;
        bool isElapsed() const;

       private:
        float seconds;
        float elapsed;
        float time;
};
}  // namespace planet
#endif