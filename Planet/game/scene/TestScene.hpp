#ifndef GAME_SCENE_TESTSCENE_HPP
#define GAME_SCENE_TESTSCENE_HPP
#include <glm/glm.hpp>
#include <string>
#include "../../gel/gel.hpp"
#define INDEX_SIZE (36)
class TestScene : public gel::IScene {
       public:
        explicit TestScene(const std::shared_ptr<gel::GameDevice>& gameDevice);
        void show() override;
        void update() override;
        void draw() override;
        void hide() override;
        bool isFinished() const override;
        std::string getNextScene() const override;

       private:
        float gameTime;
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
        glm::vec3 lightPos;
        std::string filename;
        gel::Shader& shader;
        gel::Plane plane;
        gel::ScreenBuffer screenBuffer;
        std::shared_ptr<gel::IModel> imodel;
        std::shared_ptr<gel::GameDevice> gameDevice;
};
#endif