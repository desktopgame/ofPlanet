#ifndef GEL_SHADER_LINE_HPP
#define GEL_SHADER_LINE_HPP
#include "Buffer.hpp"
#include "NameRule.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

namespace gel {
class Line {
       public:
        explicit Line(Shader& shader, const NameRule& nameRule);
        void init(const glm::vec4 start, const glm::vec4 end, glm::vec4 color);
        void update(const glm::vec4 start, const glm::vec4 end);
        void destroy();
        void draw();
        float lineWidth;

       private:
        glm::vec4 start;
        glm::vec4 end;
        glm::vec4 color;
        Shader& shader;
        NameRule nameRule;
        VertexArray vao;
        Buffer<float> vertex;
};
}  // namespace gel
#endif