#include "RightHandUI.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/raw_data.hpp>
#include "../../gel/asset/AssetDatabase.hpp"
#include "../../gel/shader/IRModel.hpp"
#include "../../gel/shader/Shader.hpp"
#include "../../gel/shader/Sprite.hpp"
RightHandUI::RightHandUI()
    : gunScrBuffer(gel::ShaderRegistry::getInstance().get("Noise"),
                   gel::NameRule()),
      gPos(0, -10, 4),
      gRot(0.0f, 0.0f, 9.2f),
      clickTimer(0.5f),
      tModel(gel::AssetDatabase::getAsset<gel::IModel>(
          "./assets/model/Gun1028.fbx")),
      bulletSprite(gel::ShaderRegistry::getInstance().get("Texture2D"), gel::NameRule()),
	  hartSprite(gel::ShaderRegistry::getInstance().get("Texture2D"), gel::NameRule()),
	  fontTable() {
        gunScrBuffer.init(gel::Game::getInstance()->getWindowWidth(),
                          gel::Game::getInstance()->getWindowHeight());
		hartSprite.init(gel::AssetDatabase::getAsset<gel::ITexture>("./assets/image/hart_gray.png")->getID(), glm::vec2(0,0),glm::vec2(64,64),1.0f);
		bulletSprite.init(gel::AssetDatabase::getAsset<gel::ITexture>("./assets/image/bullet_gray.png")->getID(), glm::vec2(0, 64), glm::vec2(64, 64), 1.0f);
		fontTable.init(std::vector<std::string>{
			"abcdefghijklmn",
			"opqrstuvwxyz.:",
			"0123456789()"
		}, [](char c, int row, int col) -> std::string {
			std::string rs = std::to_string(row);
			std::string cs = std::to_string(col);
			return "./assets/image/font/ascii/FontTable64_" + rs + "_" + cs + ".png";
		});
}
void RightHandUI::reset() {
        this->gunCache = false;
        this->clicked = false;
}
void RightHandUI::update() {
        // start fire timer
        if (gel::Input::isMousePress(gel::MouseButton::Left) && !clicked) {
                this->clicked = true;
                this->startGPos = gPos;
                this->gunCache = false;
                clickTimer.reset();
        }
        if (clicked) {
                this->gunCache = false;
                clickTimer.update();
                const float HEIGHT = 1.0f;
                float par = clickTimer.progress01();
                if (par >= 0.5f) {
                        par -= 0.5f;
                        gPos.y -= (HEIGHT * par);
                } else {
                        gPos.y += (HEIGHT * par);
                }
                if (clickTimer.isElapsed()) {
                        this->clicked = false;
                        this->gPos = startGPos;
                }
        }
}
void RightHandUI::draw(std::weak_ptr<gel::Camera> cameraRef) {
        batch(cameraRef);
        gunScrBuffer.render();
		hartSprite.draw(cameraRef.lock());
		bulletSprite.draw(cameraRef.lock());
		fontTable.draw(cameraRef.lock(), glm::vec2(64, 0), glm::vec2(38, 0), "100");
		fontTable.draw(cameraRef.lock(), glm::vec2(64, 64), glm::vec2(38, 0), "128");
}
// private
void RightHandUI::batch(std::weak_ptr<gel::Camera> cameraRef) {
        auto texShader = gel::ShaderRegistry::getInstance().get("TextureFixed");
        auto camera = cameraRef.lock();
        if (gunCache) {
                return;
        }
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