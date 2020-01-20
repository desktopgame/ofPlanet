#ifndef GEL_SHADER_SHADER_HPP
#define GEL_SHADER_SHADER_HPP
#include <ofShader.h>
#include <ofTypes.h>

#include <string>

#include "Flag.hpp"
namespace planet {
/**
 * Shader は、ofShaderのラッパーです。
 */
class Shader {
       public:
        explicit Shader();
        ~Shader();

        void load(const std::string& vertShader, const std::string& fragShader);
        void use();
        void unuse();

        // Unit
        void setUniform1f(const std::string& uniform, float a) const;
        void setUniform1fv(const std::string& uniform, int count,
                           float* v) const;
        void setUniform1i(const std::string& uniform, int a) const;
        void setUniform1iv(const std::string& uniform, int count, int* v) const;

        // Vec2
        void setUniform2f(const std::string& uniform, float a, float b) const;
        void setUniform2fv(const std::string& uniform, int count,
                           float* v) const;

        void setUniform2i(const std::string& uniform, int a, int b) const;
        void setUniform2iv(const std::string& uniform, int count, int* v) const;

        // Vec3
        void setUniform3f(const std::string& uniform, float a, float b,
                          float c) const;
        void setUniform3fv(const std::string& uniform, int count,
                           float* v) const;

        void setUniform3i(const std::string& uniform, int a, int b,
                          int c) const;
        void setUniform3iv(const std::string& uniform, int count, int* v) const;

        // Vec4
        void setUniform4f(const std::string& uniform, float a, float b, float c,
                          float d) const;
        void setUniform4fv(const std::string& uniform, int count,
                           float* v) const;

        void setUniform4i(const std::string& uniform, int a, int b, int c,
                          int d) const;
        void setUniform4iv(const std::string& uniform, int count, int* v) const;

        // Matrix
        void setUniformMatrix2fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const;
        void setUniformMatrix3fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const;
        void setUniformMatrix4fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const;

        void setUniformMatrix2f(const std::string& uniform, int count,
                                GLboolean transpose, const glm::mat2& m) const;
        void setUniformMatrix3f(const std::string& uniform, int count,
                                GLboolean transpose, const glm::mat3& m) const;
        void setUniformMatrix4f(const std::string& uniform, int count,
                                GLboolean transpose, const glm::mat4& m) const;

        void setVertexAttribPointer(const std::string& attrib, int size,
                                    GLenum type, GLboolean normalized,
                                    int stride, const GLvoid* pointer) const;
        void enableVertexAttribArray(const std::string& attrib) const;

        void setVertexAttribDivisor(const std::string& attrib, GLuint divisor);

        GLint getAttribLocation(const std::string& attrib) const;
        GLint getUniformLocation(const std::string& uniform) const;
        GLuint getProgram() const;

       private:
        Shader(const Shader& obj) = delete;
        Shader& operator=(const Shader&) = delete;
        void checkShaderError(GLuint shader, const std::string& baseStr);
        void checkProgramError(GLuint shader, const std::string& baseStr);
        Flag linkFlag;
        Flag useFlag;
        ofShader shader;
        GLuint program;
};
}  // namespace planet
#endif