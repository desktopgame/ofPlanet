#include "MouseScroll.hpp"
#include "../../gel/gel.hpp"
MouseScroll::MouseScroll(double factor)
    : oldX(-1), oldY(-1), diffX(0), diffY(0), factor(factor) {}

void MouseScroll::update() {
        this->diffX = 0;
        this->diffY = 0;
        double dmx, dmy;
        auto mWindow = gel::Game::getInstance()->getWindow();
        glfwGetCursorPos(mWindow, &dmx, &dmy);
        if (oldX == -1 && oldY == -1) {
                this->oldX = dmx;
                this->oldY = dmy;
        } else {
                this->diffX = -(dmx - oldX);
                this->diffY = -(dmy - oldY);
                this->oldX = dmx;
                this->oldY = dmy;
        }
}
void MouseScroll::reset() {
        this->oldX = -1;
        this->oldY = -1;
}
double MouseScroll::getScrollX() const { return diffX * factor; }
double MouseScroll::getScrollY() const { return diffY * factor; }