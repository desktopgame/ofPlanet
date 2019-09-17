#pragma once
#ifndef GAME_UI_STATUSUI_HPP
#define GAME_UI_STATUSUI_HPP
#include "../../gel/shader/FontTable.hpp"
#include "../../gel/shader/Sprite.hpp"
#include "../../gel/gsystem/Camera.hpp"

class StatusUI {
public:
	explicit StatusUI();
	void init();
	void destroy();
	void draw(std::shared_ptr<gel::Camera> camera);
private:
	gel::FontTable fontTable;
	gel::Sprite hartSprite;
	gel::Sprite bulletSprite;
};
#endif