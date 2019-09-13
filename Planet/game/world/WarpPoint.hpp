#ifndef GAME_WORLD_WARPPOINT_HPP
#define GAME_WORLD_WARPPOINT_HPP
#include <vector>
#include "../../gel/gel.hpp"

class WarpPoint {
       public:
        explicit WarpPoint(const std::shared_ptr<gel::Shader>& shader,
                           const gel::NameRule& nameRule);
        void init(const glm::vec4 basePoint);
        void destroy();
        void draw();
        bool isHit(const glm::vec3 position) const;

       private:
        static const float SCALE;
        glm::vec4 basePoint;
        std::shared_ptr<gel::Shader> shader;
        gel::NameRule nameRule;
        std::vector<gel::Circle> circles;
};
#endif