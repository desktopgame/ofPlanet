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
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
        glm::vec3 lightPos;
        gel::Shader& shader;
        gel::Plane plane;
        std::shared_ptr<gel::GameDevice> gameDevice;
};
#endif