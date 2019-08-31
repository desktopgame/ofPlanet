#ifndef GEL_SHADER_VERTEXARRAY_HPP
#define GEL_SHADER_VERTEXARRAY_HPP
#include "../gli.hpp"
#include "../util/Flag.hpp"

namespace gel {
class VertexArray {
       public:
        explicit VertexArray();
        void init();
        void destroy();
        void bind();
        void unbind();

       private:
        GLuint vao;
        Flag initFlag;
        Flag boundFlag;
};
}  // namespace gel
#endif