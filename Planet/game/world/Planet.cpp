#include "Planet.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "../def.hpp"
#include "../world/MultiBlock.hpp"
#include "../world/biome/BasicBiome.hpp"
#include "../world/biome/DesertBiome.hpp"
#include "../world/biome/HillBiome.hpp"
#include "../world/biome/PlainBiome.hpp"
#include "../world/gen/Generator.hpp"

float Planet::EYE_HEIGHT = 2.5f;
float Planet::MOVE_SPEED = 0.20f;
float Planet::JUMP_SPEED = 0.60f;
float Planet::ROTATE_SPEED = 4.0f;
Planet::Planet(const std::shared_ptr<gel::Shader>& shader)
    : shader(shader),
      world(shader, WSIZE_X, WSIZE_Y, WSIZE_Z),
      playerEntity(std::make_shared<EntityPhysics>()),
      playerCamera(std::make_shared<gel::Camera>()),
      diffuse{0.5f, 0.5f, 0.5f, 1.0f},
      specular{0.5f, 0.5f, 0.5f, 1.0f},
      ambient{0.5f, 0.5f, 0.5f, 1.0f},
      worldAABB(0, 0, 0, (WSIZE_X - 1), (WSIZE_Y - 1), (WSIZE_Z - 1)),
      mouseScroll(0.1),
      keyMove(playerEntity->transform, MOVE_SPEED, JUMP_SPEED),
      _isPause(false),
      random(),
      biomeVec(),
      beamLine(gel::ShaderRegistry::get("Color"),
               gel::NameRule()),
      beamDraw(false),
      beamTime(),
      mouseTrigger(GLFW_MOUSE_BUTTON_LEFT) {
        shader->use();
        shader->setUniform1i("uTexture", 0);
        shader->setUniform1f("uShininess", 50.0f);
        shader->setUniform4fv("uDiffuse", 1, diffuse);
        shader->setUniform4fv("uSpecular", 1, specular);
        shader->setUniform4fv("uAmbient", 1, ambient);
        shader->setUniform4f("uLightPos", 64, 32, 64, 1);
        shader->unuse();
        biomeVec.push_back(std::make_unique<HillBiome>());
        biomeVec.push_back(std::make_unique<HillBiome>());
        biomeVec.push_back(std::make_unique<PlainBiome>());
        biomeVec.push_back(std::make_unique<PlainBiome>());
        biomeVec.push_back(std::make_unique<DesertBiome>());
        world.setPlayer(playerEntity);
        world.setCamera(playerCamera);
        beamLine.init(glm::vec4(), glm::vec4(), glm::vec4(1.0f, 0, 0, 1));
}

void Planet::update() {
        if (_isPause) {
                return;
        }
/*
        mouseTrigger.update();
        float delta = gel::Game::getInstance()->getDeltaTime();
        if (beamDraw) {
                this->beamTime += delta;
                if (beamTime > 1.0f) {
                        this->beamDraw = false;
                }
        }
*/
#if DEBUG
        GLFWwindow* w = gel::Game::getInstance()->getWindow();
        if (glfwGetKey(w, 'D') == GLFW_PRESS &&
            glfwGetKey(w, 'G') == GLFW_PRESS) {
                generate();
                return;
        }
#endif
        world.update();
        // move player
        auto mWindow = gel::Game::getInstance()->getWindow();
        keyMove.update(playerEntity->isGround());
        mouseScroll.update();
        auto velocity = keyMove.getVelocity();
        playerEntity->transform.rotation.x += mouseScroll.getScrollX();
        playerEntity->transform.rotation.y += mouseScroll.getScrollY();
        playerEntity->transform.rotation.y =
            std::clamp(playerEntity->transform.rotation.y, -90.0f, 90.0f);
        playerEntity->velocity.x = velocity.x;
        if (playerEntity->transform.rotation.x > 360) {
                playerEntity->transform.rotation.x -= 360;
        }
        if (playerEntity->transform.rotation.y > 360) {
                playerEntity->transform.rotation.y -= 360;
        }
        if (playerEntity->transform.rotation.z > 360) {
                playerEntity->transform.rotation.z -= 360;
        }
        // jump player
        if (keyMove.isJumping()) {
                playerEntity->velocity.y = velocity.y;
        }
        // fire
        if (mouseTrigger.isEnabled()) {
                this->beamDraw = true;
                this->beamTime = 0;
        }
        /*
                if (beamDraw) {
                        auto start = glm::vec4(playerEntity->transform.position,
           1); start.y -= 1.5f; beamLine.update( start, start +
                                glm::vec4((playerEntity->transform.forward() *
           128.0f), 0));
                }
        */
        playerEntity->velocity.z = velocity.z;
        playerEntity->update(world);
        playerEntity->transform.position =
            worldAABB.clamp(playerEntity->transform.position);
        playerCamera->transform = playerEntity->transform;
        playerCamera->transform.position += (glm::vec3(0, 1, 0) * EYE_HEIGHT);
}

void Planet::draw() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // calculate matrix
        glm::vec2 windowSize = gel::Game::getInstance()->getWindowSize();
        glViewport(0, 0, windowSize.x, windowSize.y);
        playerCamera->screenWidth = windowSize.x;
        playerCamera->screenHeight = windowSize.y;
        playerCamera->calculate();
        shader->use();
        shader->setUniform4fv("uDiffuse", 1, diffuse);
        shader->setUniform4fv("uSpecular", 1, specular);
        shader->setUniform4fv("uAmbient", 1, ambient);
        shader->setUniformMatrix4fv("uMVPMatrix", 1, GL_FALSE,
                                    glm::value_ptr(playerCamera->getMVP()));
        shader->setUniformMatrix4fv("uNormalMatrix", 1, GL_FALSE,
                                    glm::value_ptr(playerCamera->getNormal()));
        shader->unuse();
        /*
                auto& beamShader =
           gel::ShaderRegistry::get("Color"); beamshader->use();
                beamshader->setUniformMatrix4fv("uMVPMatrix", 1, GL_FALSE,
                                               glm::value_ptr(playerCamera->getMVP()));
                beamshader->unuse();
        */
        world.draw();
/*
        if (beamDraw) {
                beamLine.draw();
        }
*/
#if DEBUG
/*
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive,
                              ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_Once);
        ImGui::Begin("Debug");
        ImGui::Checkbox("debug mode", &debugMode);
        ImGui::SliderFloat4("position", debugPos, 0, 128);
        ImGui::SliderFloat4("rotation", debugRot, 0, 360);
        if (ImGui::Button("debug view")) {
                debugPos[0] = 68;
                debugPos[1] = 113;
                debugPos[2] = 57;
                debugPos[3] = 0;
                debugRot[0] = 0;
                debugRot[1] = 272;
                debugRot[2] = 9;
                debugRot[3] = 0;
                debugMode = true;
        }
        if (ImGui::Button("generate")) {
                generate();
                debugMode = false;
                resume();
        }

        ImGui::End();

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        gel::gui::render();
*/
#endif
}

void Planet::pause(bool b) {
        if (b) {
                pause();
        } else {
                resume();
        }
}

void Planet::pause() {
        glfwSetInputMode(gel::Game::getInstance()->getWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_NORMAL);
        this->_isPause = true;
}

void Planet::resume() {
        glfwSetInputMode(gel::Game::getInstance()->getWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);
        this->_isPause = false;
}

bool Planet::isPause() const { return _isPause; }

void Planet::generate() {
        biomeVec.at(random.generate(0, biomeVec.size() - 1))
            ->generate(world, random);
        world.clearBatch();
        respawn();
}

void Planet::respawn() {
        int y = world.getGroundY(WSIZE_HALF_X, WSIZE_HALF_Z);
        playerEntity->transform.position =
            glm::vec3(WSIZE_HALF_X * 0.5f, (float)y, WSIZE_HALF_Z * 0.5f);
        playerEntity->velocity = glm::vec3();
}

const std::shared_ptr<gel::Camera> Planet::getCamera() const {
        return playerCamera;
}

World& Planet::getWorld() { return world; }

std::shared_ptr<EntityPhysics> Planet::getPlayer() const {
        return playerEntity;
}
