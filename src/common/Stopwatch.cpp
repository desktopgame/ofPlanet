#include "Stopwatch.hpp"
namespace planet {

Stopwatch::Stopwatch(const std::string& name)
    : name(name), a(), b(), waitStart(true), waitStop(false), logged(false) {}

Stopwatch::~Stopwatch() {}

void Stopwatch::start() {
        if (!waitStart) {
                throw std::logic_error("invalid state");
        }
        this->waitStart = false;
        this->waitStop = true;
        this->a = std::chrono::system_clock::now();
}

void Stopwatch::stop() {
        if (!waitStop || waitStart) {
                throw std::logic_error("invalid state");
        }
        this->waitStop = false;
        this->waitStart = true;
        this->b = std::chrono::system_clock::now();
}
}  // namespace planet
