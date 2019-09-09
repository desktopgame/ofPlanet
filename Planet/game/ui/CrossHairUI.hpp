#ifndef GAME_UI_CROSSHAIRUI_HPP
#define GAME_UI_CROSSHAIRUI_HPP
#include <string>
#include "../../gel/gel.hpp"
class CrossHairUI {
       public:
        explicit CrossHairUI();
        ~CrossHairUI();
        void draw(const std::shared_ptr<gel::Camera>& camera);

       private:
        gel::Sprite sprite;
        std::string path;
};
#endif