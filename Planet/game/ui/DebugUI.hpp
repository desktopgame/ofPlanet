#ifndef GAME_UI_DEBUGUI_HPP
#define GAME_UI_DEBUGUI_HPP
#include "../../gel/gel.hpp"
#include "../world/World.hpp"
class DebugUI {
       public:
        explicit DebugUI(
            const std::shared_ptr<gel::TextureManager>& textureManager,
            World& world, gel::Transform& playerTransform);
        void draw();

       private:
        void initText(
            const std::shared_ptr<gel::TextureManager>& textureManager,
            gel::Text& text);
        World& world;
        gel::Transform& playerTransform;
        gel::Text posText;
        gel::Text angleText;
        gel::Text blocksText;
        gel::Text facesText;
};
#endif