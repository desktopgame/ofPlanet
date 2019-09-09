#include "TitleScene.hpp"
#include "../../gel/device/AssetDatabase.hpp"
#include "../resources.hpp"

TitleScene::TitleScene(const std::shared_ptr<gel::GameDevice>& gameDevice)
    : gameDevice(gameDevice),
      finished(false),
      screenBuffer(gel::ShaderRegistry::getInstance().get("Screen"),
                   gel::NameRule(), gel::Game::getInstance()->getWindowWidth(),
                   gel::Game::getInstance()->getWindowHeight()),
      titleSprite(gel::ShaderRegistry::getInstance().get("Texture2D")),
      playSprite(gel::ShaderRegistry::getInstance().get("Texture2D")),
      optionSprite(gel::ShaderRegistry::getInstance().get("Texture2D")),
      exitSprite(gel::ShaderRegistry::getInstance().get("Texture2D")),
      camera(std::make_shared<gel::Camera>()) {
        screenBuffer.init();
        titleSprite.init(
            gel::AssetDatabase::getAsset<gel::ITexture>(res::TITLE_IMG_PATH)
                ->getID(),
            res::TITLE_IMG_POS, res::TITLE_IMG_SIZE, 1.0f);
        playSprite.init(
            gel::AssetDatabase::getAsset<gel::ITexture>(res::PLAY_Y_IMG_PATH)
                ->getID(),
            gel::AssetDatabase::getAsset<gel::ITexture>(res::PLAY_IMG_PATH)
                ->getID(),
            res::PLAY_IMG_POS, res::PLAY_IMG_SIZE);
        optionSprite.init(
            gel::AssetDatabase::getAsset<gel::ITexture>(res::OPTION_Y_IMG_PATH)
                ->getID(),
            gel::AssetDatabase::getAsset<gel::ITexture>(res::OPTION_IMG_PATH)
                ->getID(),
            res::OPTION_IMG_POS, res::OPTION_IMG_SIZE);
        exitSprite.init(
            gel::AssetDatabase::getAsset<gel::ITexture>(res::EXIT_Y_IMG_PATH)
                ->getID(),
            gel::AssetDatabase::getAsset<gel::ITexture>(res::EXIT_IMG_PATH)
                ->getID(),
            res::EXIT_IMG_POS, res::EXIT_IMG_SIZE);
}

TitleScene::~TitleScene() {
        screenBuffer.destroy();
        titleSprite.destroy();
        playSprite.destroy();
        optionSprite.destroy();
        exitSprite.destroy();
}

void TitleScene::show() {
        glfwSetInputMode(gel::Game::getInstance()->getWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_NORMAL);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void TitleScene::update() {
        playSprite.update();
        // optionSprite.update();
        exitSprite.update();
        if (playSprite.isActive()) {
                this->finished = true;
        }
        if (exitSprite.isActive()) {
                glfwSetWindowShouldClose(gel::Game::getInstance()->getWindow(),
                                         GL_TRUE);
        }
}

void TitleScene::draw() {
        // calculate matrix
        glm::vec2 windowSize = gel::Game::getInstance()->getWindowSize();
        glm::vec2 solutionSize = gel::Game::getInstance()->getSolutionSize();
        glViewport(0, 0, windowSize.x, windowSize.y);
        camera->screenWidth = windowSize.x;
        camera->screenHeight = windowSize.y;
        camera->calculate();
        screenBuffer.bind();
        titleSprite.draw(camera);
        playSprite.draw(camera);
        // optionSprite.draw(camera);
        exitSprite.draw(camera);
        screenBuffer.unbind();
        screenBuffer.render();
}

void TitleScene::hide() { this->finished = false; }

std::string TitleScene::getNextScene() const { return "tutorial"; }

bool TitleScene::isFinished() const { return finished; }
