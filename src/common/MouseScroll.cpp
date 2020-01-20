#include "MouseScroll.hpp"

#include "glfw.hpp"
#include <ofBaseApp.h>
#include <ofEvents.h>
namespace planet {

MouseScroll::MouseScroll()
    : oldMousePos(-1, -1), diffMousePos(), enabled(false) {}

void MouseScroll::enable() {
        if (enabled) {
                return;
        }
        this->enabled = true;
        //glfw::hideCursor();
}

void MouseScroll::disable() {
        if (!enabled) {
                return;
        }
        this->enabled = false;
        //glfw::showCursor();
}

void MouseScroll::update() {
        // check mouse
        diffMousePos.x = diffMousePos.y = 0;
		double dmx = ofGetMouseX();
		double dmy = ofGetMouseY();
        if (oldMousePos.x == -1 && oldMousePos.y == -1) {
                this->oldMousePos.x = dmx;
                this->oldMousePos.y = dmy;
        } else {
                this->diffMousePos.x = -(dmx - oldMousePos.x);
                this->diffMousePos.y = -(dmy - oldMousePos.y);
                this->oldMousePos.x = dmx;
                this->oldMousePos.y = dmy;
        }
}

glm::vec2 MouseScroll::getAmount() const { return this->diffMousePos; }
}  // namespace planet
