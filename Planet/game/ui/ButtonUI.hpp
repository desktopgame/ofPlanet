#ifndef GAME_UI_BUTTONUI_HPP
#define GAME_UI_BUTTONUI_HPP
#include <glm/glm.hpp>
#include <string>
#include "../../gel/gel.hpp"

class ButtonUI {
       public:
        explicit ButtonUI(gel::Shader& shader);
        void init(const GLuint onTexture, const GLuint offTexture,
                  const glm::vec2 pos, const glm::vec2 size);
        void destroy();
        void update();
        void draw(const std::shared_ptr<gel::Camera>& camera);
        bool isActive() const;

       private:
        gel::Sprite onSprite;
        gel::Sprite offSprite;
        gel::MouseTrigger mouseTrigger;
        bool active;
};
#endif