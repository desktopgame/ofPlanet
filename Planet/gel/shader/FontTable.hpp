#pragma once
#ifndef GEL_SHADER_FONTTABLE_HPP
#define GEL_SHADER_FONTTABLE_HPP
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "../asset/ITexture.hpp"
#include "../gsystem/Camera.hpp"
#include "Sprite.hpp"

namespace gel {
class FontTable {
       public:
        using Rule = std::function<std::string(char, int, int)>;
        explicit FontTable();
        void init(const std::vector<std::string> lines, Rule rule);
        void destroy();
        void draw(std::shared_ptr<Camera> camera, glm::vec2 pos, char c);
        void draw(std::shared_ptr<Camera> camera, glm::vec2 pos,
                  glm::vec2 interval, const std::string str);

       private:
        std::unordered_map<char, gel::Sprite> texMap;
};
}  // namespace gel
#endif