#include "CrossHairUI.hpp"
CrossHairUI::CrossHairUI(
    const std::shared_ptr<gel::TextureManager>& textureManager)
    : textureManager(textureManager),
      path("./assets/image/CrossHair.png"),
      sprite(gel::ShaderRegistry::getInstance().get("Texture2D")) {
        auto wsize = gel::Game::getInstance()->getWindowSize();
        sprite.init(textureManager->getTexture(path)->getID(),
                    glm::vec2((wsize.x - 32) / 2, (wsize.y - 32) / 2),
                    glm::vec2(32, 32), 1);
}

CrossHairUI::~CrossHairUI() { sprite.destroy(); }

void CrossHairUI::draw(const std::shared_ptr<gel::Camera>& camera) {
        sprite.draw(camera);
}