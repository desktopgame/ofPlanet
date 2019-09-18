#pragma once
#ifndef GAME_SCENE_LOADSCENE_HPP
#define GAME_SCENE_LOADSCENE_HPP
#include "../../gel/gel.hpp"
#include "../../gel/shader/FontTable.hpp"
#include "../ui/CrossHairUI.hpp"
#include "../world/Planet.hpp"
#include "../world/WarpPoint.hpp"

class LoadScene : public gel::IScene {
       public:
        explicit LoadScene();
        ~LoadScene();
        void show() override;
        void update() override;
        void draw() override;
        void hide() override;
        std::string getNextScene() const override;
        bool isFinished() const override;

       private:
        gel::FontTable fontTable;
        std::vector<gel::Sprite> sprites;
        void addSprite(std::shared_ptr<gel::ITexture> texture);
        bool finished;
        gel::Timer timer;
        std::shared_ptr<gel::Camera> camera;
        int index;
};
#endif