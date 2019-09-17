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
      fireSignal() {
        gunScrBuffer.init(gel::Game::getInstance()->getWindowWidth(),
                          gel::Game::getInstance()->getWindowHeight());
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
				fireSignal();
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
}
gel::Signal<>& RightHandUI::onFire()
{
	return fireSignal;
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