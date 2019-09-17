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
PlayScene::PlayScene()
    : planet(gel::ShaderRegistry::getInstance().get("Texture3D")),
      eKeyTrigger('E'),
      crossHair(),
      screenBuffer(gel::ShaderRegistry::getInstance().get("Noise"),
                   gel::NameRule()),
      gunScrBuffer(gel::ShaderRegistry::getInstance().get("Noise"),
                   gel::NameRule()),
      skybox(gel::ShaderRegistry::getInstance().get("SkyBox"), gel::NameRule()),
      warp(gel::ShaderRegistry::getInstance().get("Color"), gel::NameRule()),
      random(),
      gPos(0, -10, 4),
      gRot(0.0f, 0.0f, 9.2f) {
        gunScrBuffer.init(gel::Game::getInstance()->getWindowWidth(),
                          gel::Game::getInstance()->getWindowHeight());
        screenBuffer.init(gel::Game::getInstance()->getWindowWidth(),
                          gel::Game::getInstance()->getWindowHeight());
        this->tModel = gel::AssetDatabase::getAsset<gel::IModel>(
            "./assets/model/Gun1028.fbx");
        gel::CubeMapDesc desc;
        desc.front = "./assets/image/skybox/SkyBoxSide.png";
        desc.back = "./assets/image/skybox/SkyBoxSide.png";
        desc.left = "./assets/image/skybox/SkyBoxSide.png";
        desc.right = "./assets/image/skybox/SkyBoxSide.png";
        desc.top = "./assets/image/skybox/SkyBoxTop.png";
        desc.bottom = "./assets/image/skybox/SkyBoxBottom.png";
        skybox.init(desc, glm::vec3(128, 64, 128), 64, 64);
}

PlayScene::~PlayScene() {
        screenBuffer.destroy();
        skybox.destroy();
}

void PlayScene::show() {
		this->gunCache = false;
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
        auto skyboxShader = gel::ShaderRegistry::getInstance().get("SkyBox");
        auto camera = planet.getCamera();
        glm::vec2 windowSize = gel::Game::getInstance()->getWindowSize();
        glm::vec2 solutionSize = gel::Game::getInstance()->getSolutionSize();
        camera->screenWidth = windowSize.x;
        camera->screenHeight = windowSize.y;
        camera->calculate();
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
        // set matrix
		if (!gunCache) {

			auto irtModel = tModel->getIRModel();
			auto pos = camera->transform.position;
			auto rot = camera->transform.rotation;
			auto look = glm::lookAt(pos, pos + camera->transform.forward(),
				glm::vec3(0, 1, 0));

			auto gModel = glm::mat4(1.0f);
			auto gTranslate = glm::translate(gModel, gPos);
			auto gRotate = glm::rotate(gModel, gRot.y, glm::vec3(0, 1, 0)) *
				glm::rotate(gModel, gRot.x, glm::vec3(1, 0, 0)) *
				glm::rotate(gModel, gRot.z, glm::vec3(0, 0, 1));
			auto gScale = glm::scale(gModel, glm::vec3(0.1f, 0.1f, 0.1f));
			gModel = gTranslate * gRotate * gScale;

			auto gView = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1),
				glm::vec3(0, -1, 0));

			irtModel->setModelMatrix(gModel);
			irtModel->setViewMatrix(gView);
			irtModel->setProjectionMatrix(camera->getProjection());
			gunScrBuffer.bind();
			texShader->use();
			texShader->setUniform4f("uLightPos", 0, 0, 0, 1.0f);
			texShader->unuse();
			irtModel->draw();
			gunScrBuffer.unbind();
			this->gunCache = true; 
		}
		// draw planet
		screenBuffer.bind();
		skybox.draw();
		planet.draw();
		warp.draw();
		
		screenBuffer.unbind();
		screenBuffer.render();
		crossHair.draw(planet .getCamera());
		gunScrBuffer.render();
		//gunScrBuffer.render();
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
