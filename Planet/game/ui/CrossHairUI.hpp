#ifndef GAME_UI_CROSSHAIRUI_HPP
#define GAME_UI_CROSSHAIRUI_HPP
#include <string>
#include "../../gel/gel.hpp"
class CrossHairUI {
       public:
        explicit CrossHairUI(
            const std::shared_ptr<gel::TextureManager>& textureManager);
        ~CrossHairUI();
        void draw(const std::shared_ptr<gel::Camera>& camera);

       private:
        gel::Sprite sprite;
        std::string path;
        std::shared_ptr<gel::TextureManager> textureManager;
};
#endif