#ifndef GEL_SHADER_SPRITE_HPP
#define GEL_SHADER_SPRITE_HPP
#include "../gsystem/Camera.hpp"
#include "../util/Flag.hpp"
#include "Buffer.hpp"
#include "NameRule.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

namespace gel {
class Sprite {
       public:
        explicit Sprite(const std::shared_ptr<Shader>& shader,
                        const NameRule nameRule);
        explicit Sprite(const std::shared_ptr<Shader>& shader);
        void init(const GLuint texture, const glm::vec2 pos,
                  const glm::vec2 size, const float alpha);
        void reshape(const glm::vec2 pos, const glm::vec2 size);
        void destroy();
        void draw();
        glm::vec2 getPosition() const;
        glm::vec2 getSize() const;

       private:
        std::shared_ptr<Shader> shader;
        NameRule nameRule;
        GLuint texture;
        VertexArray vao;
        Flag initFlag;
        glm::mat4 projection;
        glm::mat4 mvp;
        glm::vec2 position;
        glm::vec2 size;
        Buffer<float> vertex;
        Buffer<float> uv;
};
}  // namespace gel
#endif