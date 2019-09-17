#pragma once
#ifndef GAME_UI_RIGHTHAND_UI_HPP
#define GAME_UI_RIGHTHAND_UI_HPP
#include <glm/glm.hpp>
#include <memory>
#include "../../gel/gel.hpp"
#include "../../gel/asset/IModel.hpp"
class RightHandUI {
public:
	explicit RightHandUI(const std::shared_ptr<gel::IModel>& tModel);
	void reset();
	void update();
	void draw(std::weak_ptr < gel::Camera> cameraRef);
private:
	void batch(std::weak_ptr < gel::Camera> cameraRef);
	glm::vec3 startGPos;
	glm::vec3 gPos;
	glm::vec3 gRot;
	std::shared_ptr<gel::IModel> tModel;
	gel::Timer clickTimer;
	bool clicked;
	gel::ScreenBuffer gunScrBuffer;
	bool gunCache;
};
#endif