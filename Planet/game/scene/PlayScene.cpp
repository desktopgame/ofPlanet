#include "PlayScene.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/raw_data.hpp>
#include <iostream>
#include "../../gel/asset/AssetDatabase.hpp"
#include "../../gel/shader/IRModel.hpp"
#include "../ui/StatusModel.hpp"
#include "../world/BlockRegistry.hpp"
#include "../world/gen/Generator.hpp"
PlayScene::PlayScene()
    : planet(gel::ShaderRegistry::getInstance().get("Texture3D")),
      eKeyTrigger('E'),
      crossHair(),
      screenBuffer(gel::ShaderRegistry::getInstance().get("Noise"),
                   gel::NameRule()),
      skybox(gel::ShaderRegistry::getInstance().get("SkyBox"), gel::NameRule()),
      warp(gel::ShaderRegistry::getInstance().get("Color"), gel::NameRule()),
      random(),
      rhUI(),
      statusUI(),
      beamLine(gel::ShaderRegistry::getInstance().get("Color"),
               gel::NameRule()) {
        screenBuffer.init(gel::Game::getInstance()->getWindowWidth(),
                          gel::Game::getInstance()->getWindowHeight());
        gel::CubeMapDesc desc;
        desc.front = "./assets/image/skybox/SkyBoxSide.png";
        desc.back = "./assets/image/skybox/SkyBoxSide.png";
        desc.left = "./assets/image/skybox/SkyBoxSide.png";
        desc.right = "./assets/image/skybox/SkyBoxSide.png";
        desc.top = "./assets/image/skybox/SkyBoxTop.png";
        desc.bottom = "./assets/image/skybox/SkyBoxBottom.png";
        skybox.init(desc, glm::vec3(128, 64, 128), 64, 64);
        statusUI.init();
        rhUI.onStartAnimation().connect([this]() {
                auto model = statusUI.getModel();
                auto ammo = model->getAmmo();
                if (ammo > 0) {
                        model->setAmmo(ammo - 1);
                        auto camera = planet.getCamera();
                        auto right = camera->transform.right();
                        auto back = camera->transform.backward();
                        auto down = glm::vec3(0, -1, 0);
                        back.x *= 5;
                        back.y *= 5;
                        back.z *= 5;
                        right.x *= 2;
                        right.y *= 2;
                        right.z *= 2;
                        down.y *= 2;
                        this->bStart = camera->transform
                                           .position /* + back + right + down*/;
                        this->bFwd = camera->transform.forward();
                        this->bEnd = camera->transform.position;
                        auto tmp = bFwd;
                        tmp.x *= 128;
                        tmp.y *= 128;
                        tmp.z *= 128;
                        bEnd += tmp;
                        beamLine.update(
                            glm::vec4(bStart.x, bStart.y, bStart.z, 1),
                            glm::vec4(bEnd.x, bEnd.y, bEnd.z, 1));
                }
        });
        rhUI.onEndAnimation().connect([this]() {
                // beamLine.update(glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0,
                // 1));
        });
        beamLine.lineWidth = 20;
        beamLine.init(glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 1),
                      glm::vec4(1, 0, 0, 1));
}

PlayScene::~PlayScene() {
        screenBuffer.destroy();
        skybox.destroy();
}

void PlayScene::show() {
        auto camera = planet.getCamera();
        glm::vec2 windowSize = gel::Game::getInstance()->getWindowSize();
        camera->screenWidth = windowSize.x;
        camera->screenHeight = windowSize.y;
        camera->calculate();
        rhUI.reset();
        this->gameTime = 0.0f;
        this->score = 0;
        glfwSetInputMode(gel::Game::getInstance()->getWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        goNextPlanet();
}
void PlayScene::update() {
        /*
        if (rhUI.isAnimationNow()) {
                auto scl = bFwd;
                scl.x *= 10;
                scl.y *= 10;
                scl.z *= 10;
                this->bStart += scl;
                this->bEnd += scl;
                beamLine.update(glm::vec4(bStart.x, bStart.y, bStart.z, 1),
        glm::vec4(bEnd.x, bEnd.y, bEnd.z, 1));
        }
        */
        // open/close inventory
        eKeyTrigger.update();
        if (eKeyTrigger.isEnabled()) {
                planet.pause(!planet.isPause());
        }
        rhUI.update();
        planet.update();
        beamLine.mvp = planet.getCamera()->getMVP();
}
void PlayScene::draw() {
        float delta = gel::Game::getInstance()->getDeltaTime();
        this->gameTime += delta;
        configureShader(delta);
        screenBuffer.bind();
        // draw game layer
        skybox.draw();
        planet.draw();
        warp.draw();
        beamLine.draw();
		crossHair.draw();
		rhUI.draw(planet.getCamera());
		statusUI.draw();
        screenBuffer.unbind();
        screenBuffer.render();
        if (noiseTime > 3.0f) {
                warp.destroy();
                goNextPlanet();
        }
}

void PlayScene::hide() {}
std::string PlayScene::getNextScene() const { return ""; }
bool PlayScene::isFinished() const { return false; }

// private
void PlayScene::configureShader(float delta) {
        auto camera = planet.getCamera();
        // apply matrix
        auto skyboxShader = gel::ShaderRegistry::getInstance().get("SkyBox");

        skyboxShader->use();
        skyboxShader->setUniformMatrix4fv(
            "uProjectionMatrix", 1, GL_FALSE,
            glm::value_ptr(camera->getProjection()));
        skyboxShader->setUniformMatrix4fv("uViewMatrix", 1, GL_FALSE,
                                          glm::value_ptr(camera->getView()));
        skyboxShader->unuse();
        auto circleShader = gel::ShaderRegistry::getInstance().get("Color");
        circleShader->use();
        circleShader->setUniformMatrix4fv("uMVPMatrix", 1, GL_FALSE,
                                          glm::value_ptr(camera->getMVP()));
        circleShader->unuse();
        auto noiseShader = gel::ShaderRegistry::getInstance().get("Noise");
        noiseShader->use();
        // check warp
        auto player = planet.getPlayer();
		noiseShader->setUniform1f("Time", gameTime);
        if (warp.isHit(player->transform.position)) {
                noiseShader->setUniform1i("enabled", 1);
                this->noiseTime += delta;
        } else {
                noiseShader->setUniform1i("enabled", 0);
                this->noiseTime = 0.0f;
        }
        noiseShader->unuse();
		auto colorShader = gel::ShaderRegistry::getInstance().get("ColorFixed");
        colorShader->use();
        colorShader->setUniformMatrix4fv("uMVPMatrix", 1, GL_FALSE,
                                         glm::value_ptr(camera->getMVP()));
        colorShader->setUniformMatrix4fv("uNormalMatrix", 1, GL_FALSE,
                                         glm::value_ptr(camera->getNormal()));
        colorShader->setUniform4f("uLightPos", 64, 48, 64, 1.0f);
        colorShader->unuse();
        auto texShader = gel::ShaderRegistry::getInstance().get("TextureFixed");
        texShader->use();
        texShader->setUniformMatrix4fv("uMVPMatrix", 1, GL_FALSE,
                                       glm::value_ptr(camera->getMVP()));
        texShader->setUniformMatrix4fv("uNormalMatrix", 1, GL_FALSE,
                                       glm::value_ptr(camera->getNormal()));
        texShader->setUniform4f("uLightPos", 64, 48, 64, 1.0f);
        texShader->unuse();
}

void PlayScene::goNextPlanet() {
        this->noiseTime = 0.0f;
        this->score++;
        planet.generate();
        int x = random.generate(32, 96);
        int z = random.generate(32, 96);
        int y = planet.getWorld().getGroundY(x, z);
        warp.init(glm::vec4((float)x * 0.5f, y, (float)z * 0.5f, 1.0f));
}
