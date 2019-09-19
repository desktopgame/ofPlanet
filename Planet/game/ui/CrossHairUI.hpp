#ifndef GAME_UI_CROSSHAIRUI_HPP
#define GAME_UI_CROSSHAIRUI_HPP
#include <string>
#include "../../gel/gel.hpp"
class CrossHairUI {
       public:
        explicit CrossHairUI();
        ~CrossHairUI();
        void draw();

       private:
        gel::Sprite sprite;
        std::string path;
};
#endif