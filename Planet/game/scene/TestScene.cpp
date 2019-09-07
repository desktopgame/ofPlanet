#include "TestScene.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../gel/gli.hpp"
#include "../../gel/shader/IRModel.hpp"
#include "../world/Block.hpp"
#include "../world/BlockRegistry.hpp"
#include "../world/Space.hpp"
TestScene::TestScene(const std::shared_ptr<gel::GameDevice>& gameDevice)
    : gameDevice(gameDevice),
      shader(gel::ShaderRegistry::getInstance().get("TextureFixed")),
      plane(shader),
      position(0, 0, 0),
      scale(0.01f, 0.01f, 0.01f),
      rotation(0, 0, 0),
      lightPos(0, 0, 0),
      filename("./assets/model/RedBox.fbx"),
      screenBuffer(gel::ShaderRegistry::getInstance().get("CRT"),
                   gel::NameRule(), gel::Game::getInstance()->getWindowWidth(),
                   gel::Game::getInstance()->getWindowHeight()) {
        plane.init(0.5f);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        screenBuffer.init();
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
void TestScene::update() {}
void TestScene::draw() {
        float delta = gel::Game::getInstance()->getDeltaTime();
        this->gameTime += delta;
        GLFWwindow* wd = gel::Game::getInstance()->getWindow();
        if (glfwGetKey(wd, GLFW_KEY_UP) == GLFW_PRESS) {
                rotation.x += 0.5f;
        } else if (glfwGetKey(wd, GLFW_KEY_DOWN) == GLFW_PRESS) {
                rotation.x -= 0.5f;
        }
        if (glfwGetKey(wd, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                rotation.y += 0.5f;
        } else if (glfwGetKey(wd, GLFW_KEY_LEFT) == GLFW_PRESS) {
                rotation.y -= 0.5f;
        }
        if (rotation.x > 360) rotation.x -= 360;
        if (rotation.y > 360) rotation.y -= 360;
        glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, 1280, 720);
        // calculate matrix
        auto model = glm::translate(glm::mat4(1.0f), position);
        model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
        model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
        model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
        model = glm::scale(model, scale);
        auto projection =
            glm::perspective(30.0f, 1280.0f / 720.0f, 1.0f, 1000.0f);
        auto view =
            glm::lookAt(glm::vec3(0, -3, 5), glm::vec3(), glm::vec3(0, 1, 0));
        auto mvp = projection * view * model;
        // set shader option
        gel::Shader& colorShader =
            gel::ShaderRegistry::getInstance().get("ColorFixed");
        gel::Shader& noiseShader =
            gel::ShaderRegistry::getInstance().get("CRT");
        colorShader.use();
        colorShader.setUniform4f("uLightPos", lightPos.x, lightPos.y,
                                 lightPos.z, 1);
        colorShader.unuse();
        shader.use();
        shader.setUniform4f("uLightPos", lightPos.x, lightPos.y, lightPos.z, 1);
        shader.unuse();
        noiseShader.use();
        noiseShader.setUniform1i("enabled", 1);
        noiseShader.setUniform1f("Time", gameTime);
        noiseShader.unuse();

        // bind matrix
        if (gel::exists(filename)) {
                this->imodel =
                    gameDevice->getModelManager()->getModel(filename);
        }
        auto ir = imodel->getIRModel();
        ir->setModelMatrix(model);
        ir->setViewMatrix(view);
        ir->setProjectionMatrix(projection);
        screenBuffer.bind();
        imodel->getIRModel()->draw();
        screenBuffer.unbind();
        screenBuffer.render();
#if DEBUG
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive,
                              ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_Once);

        ImGui::Begin("config 1");

        char buf[512];
        std::memset(buf, '\0', 512);
        sprintf(buf, "%s", filename.c_str());
        ImGui::InputText("File", buf, 512);
        this->filename = buf;

        ImGui::SliderFloat3("Translate", &position.x, -10, 10);
        ImGui::SliderFloat3("Rotation", &rotation.x, 0, 360);
        ImGui::SliderFloat3("Scale", &scale.x, 0, 0.5f);
        ImGui::SliderFloat3("Light", &lightPos.x, -10, 10);
        ImGui::End();

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        gel::gui::render();
#endif
}
void TestScene::hide() {}
bool TestScene::isFinished() const { return false; }
std::string TestScene::getNextScene() const { return ""; }
