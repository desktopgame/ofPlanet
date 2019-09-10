#include "LoadScene.hpp"
#include "../../gel/asset/AssetDatabase.hpp"

LoadScene::LoadScene(const std::shared_ptr<gel::GameDevice>& gameDevice)
    : gameDevice(gameDevice), finished(false), sprites(), timer(0.06f), camera(std::make_shared<gel::Camera>()),
     background(gel::ShaderRegistry::getInstance().get("Texture2D"), gel::NameRule()) {
	addSprite(gel::AssetDatabase::getAsset<gel::ITexture>("./assets/image/Progress00.png"));
	addSprite(gel::AssetDatabase::getAsset<gel::ITexture>("./assets/image/Progress01.png"));
	addSprite(gel::AssetDatabase::getAsset<gel::ITexture>("./assets/image/Progress02.png"));
	addSprite(gel::AssetDatabase::getAsset<gel::ITexture>("./assets/image/Progress03.png"));
	auto bgTex = gel::AssetDatabase::getAsset<gel::ITexture>("./assets/image/Loading.png");
	background.init(bgTex->getID(), glm::vec2(0, 0), glm::vec2(1280, 720), 1.0f);
}

LoadScene::~LoadScene() {}

void LoadScene::show() {
	this->index = 0;
}

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
	background.draw(camera);
	sprites[index].draw(camera);
}

void LoadScene::hide() {}

std::string LoadScene::getNextScene() const { return "play"; }

bool LoadScene::isFinished() const { return finished; }

void LoadScene::addSprite(std::shared_ptr<gel::ITexture> texture)
{
	auto wnd = gel::Game::getInstance()->getWindowSize();
	auto img = glm::vec2(64, 64);
	gel::Sprite sprite = gel::Sprite(gel::ShaderRegistry::getInstance().get("Texture2D"), gel::NameRule());
	sprite.init(texture->getID(), (wnd - img) / 2.0f, img, 1.0f);
	sprites.push_back(sprite);
}
