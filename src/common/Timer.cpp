#include "Timer.hpp"

#include <ofAppRunner.h>
namespace planet {

Timer::Timer(float seconds)
    : seconds(seconds), elapsed(0), time(ofGetElapsedTimef()) {}

void Timer::update() {
        float nextTime = ofGetElapsedTimef();
        float delta = nextTime - time;
        this->time = nextTime;

        this->elapsed += delta;
}

void Timer::reset() { this->elapsed = 0; }

float Timer::parcent() const { return elapsed / seconds; }

bool Timer::isElapsed() const { return elapsed >= seconds; }
}  // namespace planet
