#pragma once
#ifndef GEL_INPUT_INPUT_HPP
#define GEL_INPUT_INPUT_HPP
#include <glm/glm.hpp>

namespace gel {
enum class MouseButton {
	Left,
	Middle,
	Right
};
enum class ButtonState {
	Press,
	Release
};
enum class KeyState {
	Press,
	Release
};
class Input {
public:
	/**
	 * @return
	 */
	static glm::vec2 getMousePosition();

	/**
	 * @param button
	 * @return
	 */
	static ButtonState getMouseState(MouseButton button);
	
	/**
	 * @param button
	 * @return
	 */
	static bool isMousePress(MouseButton button);

	/**
	 * @param button
	 * @return
	 */
	static bool isMouseRelease(MouseButton button);

	/**
	 * @param keyCode
	 * @return
	 */
	static KeyState getKeyState(int keyCode);

	/**
	 * @param keyCode
	 * @return
	 */
	static bool isKeyPress(int keyCode);

	/**
	 * @param keyCode
	 * @return
	 */
	static bool isKeyRelease(int keyCode);
private:
	Input() = delete;
	~Input() = delete;
};
}
#endif