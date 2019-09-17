#include "PlayScene.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/raw_data.hpp>
#include <iostream>
#include "../../gel/asset/AssetDatabase.hpp"
#include "../../gel/shader/IRModel.hpp"
#include "../world/BlockRegistry.hpp"
#include "../world/gen/Generator.hpp"
#include "../ui/StatusModel.hpp"
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
	   beamLine(gel::ShaderRegistry::getInstance().get("Color"), gel::NameRule()) {
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
		rhUI.onFire().connect([&]() {
			auto model = statusUI.getModel();
			auto ammo = model->getAmmo();
			if (ammo > 0) {
				model->setAmmo(ammo - 1);
				auto player = planet.getPlayer();
				auto pos = player->transform.position;
				auto fwd = player->transform.forward();
				fwd.x *= 128;
				fwd.y *= 128;
				fwd.z *= 128;
				auto end = pos + fwd;
				beamLine.update(glm::vec4(pos.x, pos.y, pos.z, 1), glm::vec4(end.x,end.y,end.z, 1));
			}
		});
		beamLine.init(glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 1), glm::vec4(1,0,0,1));
}

PlayScene::~PlayScene() {
        screenBuffer.destroy();
        skybox.destroy();
}

void PlayScene::show() {
		auto camera = planet.getCamera();
		glm::vec2 windowSize = gel::Game::getInstance()->getWindowSize();
		glm::vec2 solutionSize = gel::Game::getInstance()->getSolutionSize();
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
        screenBuffer.unbind();
        screenBuffer.render();
        // draw gun layer
        crossHair.draw(planet.getCamera());
        rhUI.draw(planet.getCamera());
        statusUI.draw(planet.getCamera());
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
        auto noiseShader = gel::ShaderRegistry::getInstance().get("CRT");
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
