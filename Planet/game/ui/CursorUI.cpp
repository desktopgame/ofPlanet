#include "CursorUI.hpp"

CursorUI::CursorUI(World& world, gel::Transform& playerTransform)
    : world(world), playerTransform(playerTransform) {}

void CursorUI::update() {
        auto mWindow = gel::Game::getInstance()->getWindow();
        auto click =
            glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        this->cursorColor = gel::Color4(1, 1, 1, 1);
        auto hit =
            world.raycast(playerTransform.position, playerTransform.forward());
        if (world.contains(hit)) {
                // std::cout << "pos: " << playerTransform.position <<
                // std::endl; std::cout << "hit: " << hit << std::endl;
                if (click) {
                        // world.setBlockAt(hit, nullptr);
                }
                this->cursorColor = gel::Color4(1, 0, 0, 1);
        }
        this->cursorAt = hit;
}

void CursorUI::draw() {}