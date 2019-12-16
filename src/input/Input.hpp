#pragma once
#ifndef INPUT_INPUT_HPP
#define INPUT_INPUT_HPP
#include "../common/GLM.hpp"

#include "ButtonState.hpp"
#include "MouseButton.hpp"
namespace planet {
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
        static ButtonState getKeyState(int keyCode);

		/**
		 * @param keyCode
		 * @return
		 */
		static bool getKey(int keyCode);

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
}  // namespace planet
#endif