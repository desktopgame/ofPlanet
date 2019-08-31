#ifndef GAME_RESOURCES_HPP
#define GAME_RESOURCES_HPP
#include <glm/glm.hpp>
#include <string>

namespace res {
extern const glm::vec2 SCREEN_SIZE;
extern const std::string TITLE_IMG_PATH;
extern const std::string PLAY_IMG_PATH;
extern const std::string PLAY_Y_IMG_PATH;
extern const std::string OPTION_IMG_PATH;
extern const std::string OPTION_Y_IMG_PATH;
extern const std::string EXIT_IMG_PATH;
extern const std::string EXIT_Y_IMG_PATH;

extern const glm::vec2 TITLE_IMG_POS;
extern const glm::vec2 TITLE_IMG_SIZE;
extern const glm::vec2 PLAY_IMG_POS;
extern const glm::vec2 PLAY_IMG_SIZE;
extern const glm::vec2 OPTION_IMG_POS;
extern const glm::vec2 OPTION_IMG_SIZE;
extern const glm::vec2 EXIT_IMG_POS;
extern const glm::vec2 EXIT_IMG_SIZE;
}  // namespace res
#endif