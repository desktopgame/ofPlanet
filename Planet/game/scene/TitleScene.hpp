#ifndef GAME_SCENE_TITLESCENE_HPP
#define GAME_SCENE_TITLESCENE_HPP
#include "../../gel/gel.hpp"
#include "../ui/ButtonUI.hpp"

class TitleScene : public gel::IScene {
       public:
        explicit TitleScene();
        ~TitleScene();
        void show() override;
        void update() override;
        void draw() override;
        void hide() override;
        std::string getNextScene() const override;
        bool isFinished() const override;

       private:
        bool finished;
        std::shared_ptr<gel::Camera> camera;
        gel::Sprite titleSprite;
        ButtonUI playSprite;
        ButtonUI optionSprite;
        ButtonUI exitSprite;
        gel::ScreenBuffer screenBuffer;
};
#endif