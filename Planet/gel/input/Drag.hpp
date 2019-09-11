#pragma once
#ifndef GEL_INPUT_DRAG_HPP
#define GEL_INPUT_DRAG_HPP
#include <glm/glm.hpp>

namespace gel {
class Drag {
public:
	explicit Drag();
	void update();
	glm::vec2 getOldPosition() const;
	glm::vec2 getNewPosition() const;
	glm::vec2 getDifference() const;
	bool isDrag() const;
private:
	bool clicked;
	glm::vec2 oldPosition;
	glm::vec2 newPosition;
};
}
#endif