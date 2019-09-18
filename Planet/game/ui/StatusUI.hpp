#pragma once
#ifndef GAME_UI_STATUSUI_HPP
#define GAME_UI_STATUSUI_HPP
#include "../../gel/gsystem/Camera.hpp"
#include "../../gel/shader/FontTable.hpp"
#include "../../gel/shader/Sprite.hpp"

class StatusModel;
class StatusUI {
       public:
        explicit StatusUI();
        void init();
        void destroy();
        void draw(std::shared_ptr<gel::Camera> camera);
        std::shared_ptr<StatusModel> getModel() const;

       private:
        std::shared_ptr<StatusModel> model;
        gel::FontTable fontTable;
        gel::Sprite hartSprite;
        gel::Sprite bulletSprite;
};
#endif