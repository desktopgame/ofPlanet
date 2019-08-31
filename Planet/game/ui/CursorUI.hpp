#ifndef GAME_UI_CURSORUI_HPP
#define GAME_UI_CURSORUI_HPP
#include "../../gel/gel.hpp"
#include "../world/World.hpp"
class CursorUI {
       public:
        explicit CursorUI(World& world, gel::Transform& playerTransform);
        void update();
        void draw();

       private:
        World& world;
        gel::Transform& playerTransform;
        gel::Color4 cursorColor;
        glm::vec3 cursorAt;
};

#endif