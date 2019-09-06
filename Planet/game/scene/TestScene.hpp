#ifndef GAME_SCENE_TESTSCENE_HPP
#define GAME_SCENE_TESTSCENE_HPP
#include <glm/glm.hpp>
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
        gel::Shader& shader;
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 mvp;
        glm::mat4 normalMatrix;
        gel::Plane plane;
        float angle;
        std::shared_ptr<gel::GameDevice> gameDevice;
};
#endif