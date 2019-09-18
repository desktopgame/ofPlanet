#ifndef GAME_SCENE_PLAYSCENE_HPP
#define GAME_SCENE_PLAYSCENE_HPP
#include "../../gel/gel.hpp"
#include "../../gel/shader/Line.hpp"
#include "../../gel/shader/PixelBuffer.hpp"
#include "../../gel/shader/RawTexture.hpp"
#include "../ui/CrossHairUI.hpp"
#include "../ui/RightHandUI.hpp"
#include "../ui/StatusUI.hpp"
#include "../world/Planet.hpp"
#include "../world/WarpPoint.hpp"

class PlayScene : public gel::IScene {
       public:
        explicit PlayScene();
        ~PlayScene();
        void show() override;
        void update() override;
        void draw() override;
        void hide() override;
        std::string getNextScene() const override;
        bool isFinished() const override;

       private:
        void configureShader(float delta);
        void goNextPlanet();
        float gameTime;
        float noiseTime;
        int score;
        std::shared_ptr<gel::ContentManager> contentManager;
        glm::vec3 bStart;
        glm::vec3 bEnd;
        glm::vec3 bFwd;
        CrossHairUI crossHair;
        Planet planet;
        WarpPoint warp;
        RightHandUI rhUI;
        StatusUI statusUI;
        gel::Line beamLine;
        gel::Random random;
        gel::CubeMap skybox;
        gel::KeyTrigger eKeyTrigger;
        gel::ScreenBuffer screenBuffer;
};
#endif