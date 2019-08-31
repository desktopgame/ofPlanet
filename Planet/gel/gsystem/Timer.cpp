#include "Timer.hpp"
#include <algorithm>
#include "../Game.hpp"
namespace gel {
Timer::Timer(float length) : length(length), elapsed(0) {}

void Timer::update() { this->elapsed += Game::getInstance()->getDeltaTime(); }
void Timer::reset() { this->elapsed = 0; }
float Timer::progress() const { return elapsed / length; }
float Timer::progress01() const {
        return std::min(1.0f, std::max(0.0f, elapsed / length));
}
bool Timer::isElapsed() const { return elapsed >= length; }
}  // namespace gel