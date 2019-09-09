#include "TestScene.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../gel/device/AssetDatabase.hpp"
#include "../../gel/gli.hpp"
#include "../../gel/shader/IRModel.hpp"
#include "../../gel/ui/imgui/ImGuizmo.h"
#include "../world/Block.hpp"
#include "../world/BlockRegistry.hpp"
#include "../world/Space.hpp"
TestScene::TestScene(const std::shared_ptr<gel::GameDevice>& gameDevice)
    : gameDevice(gameDevice),
      shader(gel::ShaderRegistry::getInstance().get("TextureFixed")),
      plane(shader),
      model(1.0f),
      filename("./assets/model/Gun1028.fbx"),
      screenBuffer(gel::ShaderRegistry::getInstance().get("CRT"),
                   gel::NameRule(), gel::Game::getInstance()->getWindowWidth(),
                   gel::Game::getInstance()->getWindowHeight()),
      sprite(gel::ShaderRegistry::getInstance().get("Texture2D")),
      camera(std::make_shared<gel::Camera>()) {
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
void TestScene::update() {}
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
#if DEBUG
        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
        static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
        if (ImGui::IsKeyPressed(90))
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(69)) mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(82))  // r Key
                mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Translate",
                               mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate",
                               mCurrentGizmoOperation == ImGuizmo::ROTATE))
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale",
                               mCurrentGizmoOperation == ImGuizmo::SCALE))
                mCurrentGizmoOperation = ImGuizmo::SCALE;
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model),
                                              matrixTranslation, matrixRotation,
                                              matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation, 3);
        ImGui::InputFloat3("Rt", matrixRotation, 3);
        ImGui::InputFloat3("Sc", matrixScale, 3);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation,
                                                matrixRotation, matrixScale,
                                                glm::value_ptr(model));

        if (mCurrentGizmoOperation != ImGuizmo::SCALE) {
                if (ImGui::RadioButton("Local",
                                       mCurrentGizmoMode == ImGuizmo::LOCAL))
                        mCurrentGizmoMode = ImGuizmo::LOCAL;
                ImGui::SameLine();
                if (ImGui::RadioButton("World",
                                       mCurrentGizmoMode == ImGuizmo::WORLD))
                        mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        static bool useSnap(false);
        if (ImGui::IsKeyPressed(83)) useSnap = !useSnap;
        ImGui::Checkbox("", &useSnap);
        ImGui::SameLine();
        glm::vec3 snap = glm::vec3(0);
        switch (mCurrentGizmoOperation) {
                case ImGuizmo::TRANSLATE:
                        // snap = config.mSnapTranslation;
                        ImGui::InputFloat3("Snap", &snap.x);
                        break;
                case ImGuizmo::ROTATE:
                        // snap = config.mSnapRotation;
                        ImGui::InputFloat("Angle Snap", &snap.x);
                        break;
                case ImGuizmo::SCALE:
                        // snap = config.mSnapScale;
                        ImGui::InputFloat("Scale Snap", &snap.x);
                        break;
        }
        ImGuiIO& io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
                             mCurrentGizmoOperation, mCurrentGizmoMode,
                             glm::value_ptr(model), NULL,
                             useSnap ? &snap.x : NULL);

        gel::gui::render();
#endif
}
void TestScene::hide() {}
bool TestScene::isFinished() const { return false; }
std::string TestScene::getNextScene() const { return ""; }
