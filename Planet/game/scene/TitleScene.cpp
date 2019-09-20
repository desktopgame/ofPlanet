#include "TitleScene.hpp"
#include "../../gel/asset/AssetDatabase.hpp"
#include "../resources.hpp"

TitleScene::TitleScene()
    : finished(false),
      screenBuffer(gel::ShaderRegistry::getInstance().get("Screen"),
                   gel::NameRule()),
      titleSprite(gel::ShaderRegistry::getInstance().get("Texture2D")),
      playSprite(gel::ShaderRegistry::getInstance().get("Texture2D")),
      optionSprite(gel::ShaderRegistry::getInstance().get("Texture2D")),
      exitSprite(gel::ShaderRegistry::getInstance().get("Texture2D")) {
        screenBuffer.init(gel::Game::getInstance()->getWindowWidth(),
                          gel::Game::getInstance()->getWindowHeight());
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
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screenBuffer.bind();
        titleSprite.draw();
        playSprite.draw();
        exitSprite.draw();
        screenBuffer.unbind();
        screenBuffer.render();
}

void TitleScene::hide() { this->finished = false; }

std::string TitleScene::getNextScene() const { return "tutorial"; }

bool TitleScene::isFinished() const { return finished; }
