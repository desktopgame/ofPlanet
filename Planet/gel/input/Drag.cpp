#include "Drag.hpp"
#include "Input.hpp"

namespace gel {
	Drag::Drag() : clicked(false)
	{
	}
	void Drag::update()
	{
		if (!clicked) {
			if (Input::isMousePress(MouseButton::Left)) {
				this->clicked = true;
				this->oldPosition = Input::getMousePosition();
				this->newPosition = oldPosition;
			}
		} else {
			if (Input::isMousePress(MouseButton::Left)) {
				this->oldPosition = this->newPosition;
				this->newPosition = Input::getMousePosition();
			}
			else {
				this->clicked = false;
			}
		}
	}
	glm::vec2 Drag::getOldPosition() const
	{
		return oldPosition;
	}
	glm::vec2 Drag::getNewPosition() const
	{
		return newPosition;
	}
	glm::vec2 Drag::getDifference() const
	{
		return newPosition - oldPosition;
	}
	bool Drag::isDrag() const
	{
		return clicked;
	}
}