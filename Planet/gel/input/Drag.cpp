#include "Drag.hpp"
#include "Input.hpp"

namespace gel {
	Drag::Drag(MouseButton button) : clicked(false), button(button)
	{
	}
	void Drag::update()
	{
		if (!clicked) {
			if (Input::isMousePress(button)) {
				this->clicked = true;
				this->oldPosition = Input::getMousePosition();
				this->newPosition = oldPosition;
			}
		} else {
			if (Input::isMousePress(button)) {
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