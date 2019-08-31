#include "ButtonUI.hpp"

ButtonUI::ButtonUI(gel::Shader& shader)
    : onSprite(shader),
      offSprite(shader),
      mouseTrigger(GLFW_MOUSE_BUTTON_LEFT),
      active(false) {}

void ButtonUI::init(const GLuint onTexture, const GLuint offTexture,
                    const glm::vec2 pos, const glm::vec2 size) {
        onSprite.init(onTexture, pos, size, 1);
        offSprite.init(offTexture, pos, size, 1);
}

void ButtonUI::destroy() {
        onSprite.destroy();
        offSprite.destroy();
}

void ButtonUI::update() { mouseTrigger.update(); }

void ButtonUI::draw(const std::shared_ptr<gel::Camera>& camera) {
        this->active = false;
        gel::Rectangle spriteRect =
            gel::Rectangle(onSprite.getPosition(), onSprite.getSize());
        double xp, yp;
        glfwGetCursorPos(gel::Game::getInstance()->getWindow(), &xp, &yp);
        if (spriteRect.contains((float)xp, (float)yp)) {
                onSprite.draw(camera);
                if (mouseTrigger.isEnabled()) {
                        this->active = true;
                }
        } else {
                offSprite.draw(camera);
        }
}

bool ButtonUI::isActive() const { return active; }
