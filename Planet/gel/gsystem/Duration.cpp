#include "Duration.hpp"
namespace gel {
Duration::Duration(float length) : timer(length) {}

void Duration::update() {
        timer.update();
        if (timer.isElapsed()) {
                timer.reset();
                this->increase = !this->increase;
        }
}

float Duration::progress() const {
        if (increase) {
                return timer.progress01();
        }
        return 1 - timer.progress01();
}
}  // namespace gel