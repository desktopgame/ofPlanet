#include "PlayScene.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/raw_data.hpp>
#include <iostream>
#include "../world/BlockRegistry.hpp"
#include "../world/gen/Generator.hpp"
PlayScene::PlayScene(const std::shared_ptr<gel::GameDevice>& gameDevice)
    : gameDevice(gameDevice),
      planet(gel::ShaderRegistry::getInstance().get("Texture3D")),
      eKeyTrigger('E'),
      crossHair(gameDevice->getTextureManager()),
      screenBuffer(gel::ShaderRegistry::getInstance().get("Noise"),
                   gel::NameRule(), gel::Game::getInstance()->getWindowWidth(),
                   gel::Game::getInstance()->getWindowHeight()),
      skybox(gel::ShaderRegistry::getInstance().get("SkyBox"), gel::NameRule()),
      warp(gel::ShaderRegistry::getInstance().get("Color"), gel::NameRule()),
      random() {
        screenBuffer.init();
        gel::CubeMapDesc desc;
        desc.front = "./assets/image/skybox/SkyBoxSide.png";
        desc.back = "./assets/image/skybox/SkyBoxSide.png";
        desc.left = "./assets/image/skybox/SkyBoxSide.png";
        desc.right = "./assets/image/skybox/SkyBoxSide.png";
        desc.top = "./assets/image/skybox/SkyBoxTop.png";
        desc.bottom = "./assets/image/skybox/SkyBoxBottom.png";
        skybox.init(gameDevice->getTextureManager(), desc,
                    glm::vec3(128, 64, 128), 64, 64);
}

PlayScene::~PlayScene() {
        screenBuffer.destroy();
        skybox.destroy();
}

void PlayScene::show() {
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
        // open/close inventory
        eKeyTrigger.update();
        if (eKeyTrigger.isEnabled()) {
                planet.pause(!planet.isPause());
        }
        planet.update();
}
void PlayScene::draw() {
        float delta = gel::Game::getInstance()->getDeltaTime();
        this->gameTime += delta;
        // apply matrix
        gel::Shader& skyboxShader =
            gel::ShaderRegistry::getInstance().get("SkyBox");
        auto camera = planet.getCamera();
        skyboxShader.use();
        skyboxShader.setUniformMatrix4fv(
            "uProjectionMatrix", 1, GL_FALSE,
            glm::value_ptr(camera->getProjection()));
        skyboxShader.setUniformMatrix4fv("uViewMatrix", 1, GL_FALSE,
                                         glm::value_ptr(camera->getView()));
        skyboxShader.unuse();
        gel::Shader& circleShader =
            gel::ShaderRegistry::getInstance().get("Color");
        circleShader.use();
        circleShader.setUniformMatrix4fv("uMVPMatrix", 1, GL_FALSE,
                                         glm::value_ptr(camera->getMVP()));
        circleShader.unuse();
        gel::Shader& noiseShader =
            gel::ShaderRegistry::getInstance().get("CRT");
        noiseShader.use();
        // check warp
        auto player = planet.getPlayer();
        noiseShader.setUniform1f("Time", gameTime);
        if (warp.isHit(player->transform.position)) {
                noiseShader.setUniform1i("enabled", 1);
                this->noiseTime += delta;
        } else {
                noiseShader.setUniform1i("enabled", 0);
                this->noiseTime = 0.0f;
        }
        noiseShader.unuse();
        // double buffered rendering
        screenBuffer.bind();
        skybox.draw();
        planet.draw();
        warp.draw();
        screenBuffer.unbind();
        screenBuffer.render();
        crossHair.draw(planet.getCamera());
        if (noiseTime > 3.0f) {
                warp.destroy();
                goNextPlanet();
        }
}

void PlayScene::hide() {}
std::string PlayScene::getNextScene() const { return ""; }
bool PlayScene::isFinished() const { return false; }

// private
void PlayScene::goNextPlanet() {
        this->noiseTime = 0.0f;
        this->score++;
        planet.generate();
        int x = random.generate(32, 96);
        int z = random.generate(32, 96);
        int y = planet.getWorld().getGroundY(x, z);
        warp.init(glm::vec4((float)x * 0.5f, y, (float)z * 0.5f, 1.0f));
}
