#ifndef GEL_SHADER_CUBEMAP_HPP
#define GEL_SHADER_CUBEMAP_HPP
#include <string>
#include "Buffer.hpp"
#include "NameRule.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

namespace gel {
struct CubeMapDesc {
        std::string right;
        std::string left;
        std::string top;
        std::string bottom;
        std::string front;
        std::string back;
};

class CubeMap {
       public:
        explicit CubeMap(const std::shared_ptr<Shader>& shader, const NameRule& nameRule);
        void init(const CubeMapDesc& desc, const glm::vec3 scale, int width,
                  const int height);
        void destroy();
        void draw();

       private:
        static GLuint loadCubeMap(const CubeMapDesc& desc, const int width,
                                  const int height);
        GLuint texture;
        std::shared_ptr<Shader> shader;
        NameRule nameRule;
        VertexArray vao;
        Buffer<float> vertex;
};
}  // namespace gel
#endif