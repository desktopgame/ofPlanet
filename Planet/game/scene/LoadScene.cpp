#include "LoadScene.hpp"

LoadScene::LoadScene(const std::shared_ptr<gel::GameDevice>& gameDevice)
    : gameDevice(gameDevice), finished(false) {}

LoadScene::~LoadScene() {}

void LoadScene::show() {}

void LoadScene::update() {}

void LoadScene::draw() {}

void LoadScene::hide() {}

std::string LoadScene::getNextScene() const { return "play"; }

bool LoadScene::isFinished() const { return finished; }
