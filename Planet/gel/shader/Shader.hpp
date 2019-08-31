#ifndef GEL_SHADER_SHADER_HPP
#define GEL_SHADER_SHADER_HPP
#include <string>
#include "../gli.hpp"
#include "../util/Flag.hpp"
namespace gel {
class Shader {
       public:
        explicit Shader();
        static Shader make(const std::string& vertShader,
                           const std::string& fragShader);
        void link(const std::string& vertShader, const std::string& fragShader);
        void unlink();
        void use();
        void unuse();

        void setUniform1f(const std::string& uniform, float a) const;
        void setUniform1fv(const std::string& uniform, int count,
                           float* v) const;
        void setUniform1i(const std::string& uniform, int a) const;
        void setUniform1iv(const std::string& uniform, int count, int* v) const;

        void setUniform2f(const std::string& uniform, float a, float b) const;
        void setUniform2fv(const std::string& uniform, int count,
                           float* v) const;

        void setUniform3f(const std::string& uniform, float a, float b,
                          float c) const;
        void setUniform3fv(const std::string& uniform, int count,
                           float* v) const;

        void setUniform4f(const std::string& uniform, float a, float b, float c,
                          float d) const;
        void setUniform4fv(const std::string& uniform, int count,
                           float* v) const;

        void setUniformMatrix2fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const;
        void setUniformMatrix3fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const;
        void setUniformMatrix4fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const;

        void setVertexAttribPointer(const std::string& attrib, int size,
                                    GLenum type, GLboolean normalized,
                                    int stride, const GLvoid* pointer) const;

        GLint getAttribLocation(const std::string& attrib) const;
        GLint getUniformLocation(const std::string& uniform) const;
        GLuint getProgram() const;

       private:
        Flag linkFlag;
        Flag useFlag;
        void checkShaderError(GLuint shader, const std::string& baseStr);
        GLuint program;
};
}  // namespace gel
#endif