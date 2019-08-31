#ifndef GAME_SCENE_TUTORIALSCENE_HPP
#define GAME_SCENE_TUTORIALSCENE_HPP
#include <vector>
#include "../../gel/gel.hpp"

class TutorialScene : public gel::IScene {
       public:
        explicit TutorialScene(
            const std::shared_ptr<gel::GameDevice>& gameDevice);
        ~TutorialScene();
        void show() override;
        void update() override;
        void draw() override;
        void hide() override;
        std::string getNextScene() const override;
        bool isFinished() const override;

       private:
        gel::MouseTrigger mouseTrigger;
        std::shared_ptr<gel::Camera> camera;
        std::shared_ptr<gel::GameDevice> gameDevice;
        std::vector<gel::Sprite> sprites;
        int page;
        bool finished;
        float elapsed;

        float wait;
        bool waitStart;
};
#endif