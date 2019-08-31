
#include "DebugUI.hpp"

DebugUI::DebugUI(const std::shared_ptr<gel::TextureManager>& textureManager,
                 World& world, gel::Transform& playerTransform)
    : posText(),
      angleText(),
      blocksText(),
      facesText(),
      world(world),
      playerTransform(playerTransform) {
        initText(textureManager, posText);
        initText(textureManager, angleText);
        initText(textureManager, blocksText);
        initText(textureManager, facesText);
        angleText.position.y = 28 * 1;
        blocksText.position.y = 28 * 2;
        facesText.position.y = 28 * 3;
}

void DebugUI::draw() {
        posText.text =
            "pos " +
            std::to_string(Block::floatToInt(playerTransform.position.x)) +
            " " +
            std::to_string(Block::floatToInt(playerTransform.position.y)) +
            " " + std::to_string(Block::floatToInt(playerTransform.position.z));
        posText.draw();
        angleText.text =
            "rotate " +
            std::to_string(Block::floatToInt(playerTransform.rotation.x)) +
            " " +
            std::to_string(Block::floatToInt(playerTransform.rotation.y)) +
            " " + std::to_string(Block::floatToInt(playerTransform.rotation.z));
        angleText.draw();
        blocksText.text =
            "draw blocks " + std::to_string(world.getVisibleBlockCount());
        blocksText.draw();
        facesText.text =
            "draw faces " + std::to_string(world.getVisibleFaceCount());
        facesText.draw();
}

void DebugUI::initText(
    const std::shared_ptr<gel::TextureManager>& textureManager,
    gel::Text& text) {
        text.putAlpha("./assets/image/text/AlphaMapFill_14x28", textureManager);
        text.putDigit("./assets/image/text/AlphaMapFill_14x28", textureManager);
        text.cellSize = glm::vec2(14, 28);
}