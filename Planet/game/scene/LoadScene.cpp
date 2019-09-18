#include "LoadScene.hpp"
#include "../../gel/asset/AssetDatabase.hpp"

LoadScene::LoadScene()
    : finished(false),
      sprites(),
      timer(0.06f),
      camera(std::make_shared<gel::Camera>()),
     fontTable() {
        addSprite(gel::AssetDatabase::getAsset<gel::ITexture>(
            "./assets/image/Progress00.png"));
        addSprite(gel::AssetDatabase::getAsset<gel::ITexture>(
            "./assets/image/Progress01.png"));
        addSprite(gel::AssetDatabase::getAsset<gel::ITexture>(
            "./assets/image/Progress02.png"));
        addSprite(gel::AssetDatabase::getAsset<gel::ITexture>(
            "./assets/image/Progress03.png"));
		fontTable.init(
			std::vector<std::string>{"abcdefghijklmn",
			"opqrstuvwxyz.:", "0123456789()", "ABCDEFGHIJKLMN", "OPQRSTUVWXYZ+-"},
			[](char c, int row, int col) -> std::string {
			std::string rs = std::to_string(row);
			std::string cs = std::to_string(col);
			return "./assets/image/font/ascii/FontTable64_" + rs + "_" +
				cs + ".png";
		});
}

LoadScene::~LoadScene() {}

void LoadScene::show() { this->index = 0; }

void LoadScene::update() {
        timer.update();
        if (timer.isElapsed()) {
                timer.reset();
                index++;
                if (index >= sprites.size()) {
                        index = 0;
                }
        }
}

void LoadScene::draw() {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto wnd = gel::Game::getInstance()->getWindowSize();
        camera->screenWidth = wnd.x;
        camera->screenHeight = wnd.y;
        camera->calculate();
		fontTable.draw(camera, glm::vec2(0, wnd.y-64), glm::vec2(38, 0), "Loading...");
        sprites[index].draw(camera);
}

void LoadScene::hide() {}

std::string LoadScene::getNextScene() const { return "play"; }

bool LoadScene::isFinished() const { return finished; }

void LoadScene::addSprite(std::shared_ptr<gel::ITexture> texture) {
        auto wnd = gel::Game::getInstance()->getWindowSize();
        auto img = glm::vec2(64, 64);
        gel::Sprite sprite =
            gel::Sprite(gel::ShaderRegistry::getInstance().get("Texture2D"),
                        gel::NameRule());
        sprite.init(texture->getID(), (wnd - img) / 2.0f, img, 1.0f);
        sprites.push_back(sprite);
}
