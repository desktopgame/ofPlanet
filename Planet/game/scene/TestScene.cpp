#include "TestScene.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../gel/asset/AssetDatabase.hpp"
#include "../../gel/gli.hpp"
#include "../../gel/shader/IRModel.hpp"
#include "../../gel/ui/imgui/ImGuizmo.h"
#include "../world/Block.hpp"
#include "../world/BlockRegistry.hpp"
#include "../world/Space.hpp"
TestScene::TestScene()
    : shader(gel::ShaderRegistry::getInstance().get("TextureFixed")),
      plane(shader),
      model(1.0f),
      filename("./assets/model/Gun1028.fbx"),
      screenBuffer(gel::ShaderRegistry::getInstance().get("CRT"),
                   gel::NameRule(), gel::Game::getInstance()->getWindowWidth(),
                   gel::Game::getInstance()->getWindowHeight()),
      sprite(gel::ShaderRegistry::getInstance().get("Texture2D")),
      camera(std::make_shared<gel::Camera>()),
      position(0),
      scale(1),
      rotation(0),
      leftDrag(gel::MouseButton::Left) {
        plane.init(0.5f);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        screenBuffer.init();
        sprite.init(gel::AssetDatabase::getAsset<gel::ITexture>(
                        "./assets/image/sample.jpg")
                        ->getID(),
                    glm::vec2(32, 32), glm::vec2(150, 146), 1.0f);
}

void TestScene::show() {
        this->gameTime = 0;
        glfwSetInputMode(gel::Game::getInstance()->getWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_NORMAL);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
}
void TestScene::update() {
        leftDrag.update();
        if (leftDrag.isDrag()) {
                this->position.x += leftDrag.getDifference().x;
                this->position.y += leftDrag.getDifference().y;
                if (rotation.x > 360) rotation.x -= 360;
                if (rotation.y > 360) rotation.y -= 360;
        }
}
void TestScene::draw() {
        float delta = gel::Game::getInstance()->getDeltaTime();
        this->gameTime += delta;
        GLFWwindow* wd = gel::Game::getInstance()->getWindow();
        glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, 1280, 720);
        // calculate camera
        glm::vec2 windowSize = gel::Game::getInstance()->getWindowSize();
        glm::vec2 solutionSize = gel::Game::getInstance()->getSolutionSize();
        glViewport(0, 0, windowSize.x, windowSize.y);
        camera->screenWidth = windowSize.x;
        camera->screenHeight = windowSize.y;
        camera->calculate();
        // calculate matrix
        this->model = glm::mat4(1.0f);
        this->model = glm::translate(model, position);
        this->model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
        this->model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
        this->model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
        this->model = glm::scale(model, scale);
        auto projection =
            glm::perspective(30.0f, 1280.0f / 720.0f, 1.0f, 1000.0f);
        auto view =
            glm::lookAt(glm::vec3(0, -3, 5), glm::vec3(), glm::vec3(0, 1, 0));
        auto mvp = projection * view * model;
        // set shader option
        auto colorShader = gel::ShaderRegistry::getInstance().get("ColorFixed");
        auto noiseShader = gel::ShaderRegistry::getInstance().get("CRT");
        colorShader->use();
        colorShader->setUniform4f("uLightPos", lightPos.x, lightPos.y,
                                  lightPos.z, 1);
        colorShader->unuse();
        shader->use();
        shader->setUniform4f("uLightPos", lightPos.x, lightPos.y, lightPos.z,
                             1);
        shader->unuse();
        noiseShader->use();
        noiseShader->setUniform1i("enabled", 1);
        noiseShader->setUniform1f("Time", gameTime);
        noiseShader->unuse();

        // bind matrix
        if (gel::exists(filename)) {
                this->imodel =
                    gel::AssetDatabase::getAsset<gel::IModel>(filename);
        }
        auto ir = imodel->getIRModel();
        ir->setModelMatrix(model);
        ir->setViewMatrix(view);
        ir->setProjectionMatrix(projection);
        //        screenBuffer.bind();
        imodel->getIRModel()->draw();
        sprite.draw(camera);
        //        screenBuffer.unbind();
        //        screenBuffer.render();
}
void TestScene::hide() {}
bool TestScene::isFinished() const { return false; }
std::string TestScene::getNextScene() const { return ""; }
