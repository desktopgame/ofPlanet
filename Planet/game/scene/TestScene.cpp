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
    : shader(gel::ShaderRegistry::get("TextureFixed")),
      plane(shader),
      model(1.0f),
      filename("./assets/model/Gun1028.fbx"),
      screenBuffer(gel::ShaderRegistry::get("CRT"),
                   gel::NameRule()),
      sprite(gel::ShaderRegistry::get("Texture2D")),
      camera(std::make_shared<gel::Camera>()),
      position(0),
      scale(1),
      rotation(0),
      leftDrag(gel::MouseButton::Left),
      box(gel::ShaderRegistry::get("ColorFixed"),
          gel::NameRule()) {
        plane.init(0.5f);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        screenBuffer.init(gel::Game::getInstance()->getWindowWidth(),
                          gel::Game::getInstance()->getWindowHeight());
        //box.init(glm::vec4(1, 0, 0, 1), glm::vec3(1, 1, 1));
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
        glm::mat4 normalMatrix = view * model;
        normalMatrix = glm::inverse(normalMatrix);
        normalMatrix = glm::transpose(normalMatrix);
        // set shader option
        auto colorShader = gel::ShaderRegistry::get("ColorFixed");
        auto noiseShader = gel::ShaderRegistry::get("CRT");
        colorShader->use();
        gel::NameRule nameRule;
        colorShader->setUniformMatrix4fv("uMVPMatrix", 1, GL_FALSE,
                                         glm::value_ptr(mvp));
        colorShader->setUniformMatrix4fv("uNormalMatrix", 1, GL_FALSE,
                                         glm::value_ptr(normalMatrix));
        colorShader->setUniform4f("uLightPos", lightPos.x, lightPos.y,
                                  lightPos.z, 1);
        colorShader->setUniform4f("uColor", 1, 0, 0, 1);
        shader->setUniform1f(nameRule.uniformShininess, 10.0f);
        shader->setUniform4f(nameRule.uniformAmbient, 0.5f, 0.5f, 0.5f, 1);
        shader->setUniform4f(nameRule.uniformDiffuse, 0.5f, 0.5f, 0.5f, 1);
        shader->setUniform4f(nameRule.uniformSpecular, 0.5f, 0.5f, 0.5f, 1);
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
        box.draw();
}
void TestScene::hide() {}
bool TestScene::isFinished() const { return false; }
std::string TestScene::getNextScene() const { return ""; }
