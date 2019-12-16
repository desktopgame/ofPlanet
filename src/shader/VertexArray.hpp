#pragma once
#ifndef SHADER_VERTEXARRAY_HPP
#define SHADER_VERTEXARRAY_HPP
#include <ofTypes.h>

#include "Flag.hpp"
namespace planet {

class VertexArray {
       public:
        explicit VertexArray();
        ~VertexArray();

        void bind();
        void unbind();

       private:
        VertexArray(const VertexArray& obj) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        Flag bindFlag;
        GLuint name;
};
}  // namespace planet
#endif  // !SHADER_VERTEXARRAY_HPP
