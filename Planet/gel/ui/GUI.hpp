#ifndef GEL_UI_GUI_HPP
#define GEL_UI_GUI_HPP
#include "../gli.hpp"
#include "imgui/imgui.h"
namespace gel {
/**
 * GUI is ui utility for game develop.
 */
namespace gui {
namespace internal {
void init(GLFWwindow* window);
void destroy();
void newFrame();
void endFrame();
}  // namespace internal

/**
 * render GUI.
 */
void render();
};  // namespace gui
}  // namespace gel
#endif