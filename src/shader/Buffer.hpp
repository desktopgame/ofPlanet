#pragma once
#ifndef SHADER_BUFFER_HPP
#define SHADER_BUFFER_HPP
#include <ofTypes.h>
#include <ofVec2f.h>
#include <ofVec3f.h>

#include <vector>

#include "Flag.hpp"
namespace planet {

template <typename T, GLenum Target, GLenum Usage>
class Buffer {
       public:
        explicit Buffer();
        ~Buffer();

        void put(const T value);
        void put(const glm::tvec2<T> value);
        void put(const glm::tvec3<T> value);

        void put(const std::vector<T> value);
        void put(const std::vector<glm::tvec2<T> > value);
        void put(const std::vector<glm::tvec3<T> > value);

        void bind();
        void update();
        void unbind();

       private:
        bool dirty;
        Flag bindFlag;
        std::vector<T> vec;
        GLuint vbo;
};

template <typename T, GLenum Target, GLenum Usage>
inline Buffer<T, Target, Usage>::Buffer()
    : dirty(true), bindFlag(false), vec(), vbo(0) {
        glGenBuffers(1, &vbo);
}

template <typename T, GLenum Target, GLenum Usage>
inline Buffer<T, Target, Usage>::~Buffer() {
        glDeleteBuffers(1, &vbo);
}

template <typename T, GLenum Target, GLenum Usage>
inline void Buffer<T, Target, Usage>::put(const T value) {
        vec.emplace_back(value);
        this->dirty = true;
}

template <typename T, GLenum Target, GLenum Usage>
inline void Buffer<T, Target, Usage>::put(const glm::tvec2<T> value) {
        put(value.x);
        put(value.y);
}

template <typename T, GLenum Target, GLenum Usage>
inline void Buffer<T, Target, Usage>::put(const glm::tvec3<T> value) {
        put(value.x);
        put(value.y);
        put(value.z);
}

template <typename T, GLenum Target, GLenum Usage>
inline void Buffer<T, Target, Usage>::put(const std::vector<T> value) {
        for (auto v : value) put(v);
}

template <typename T, GLenum Target, GLenum Usage>
inline void Buffer<T, Target, Usage>::put(
    const std::vector<glm::tvec2<T> > value) {
        for (auto v : value) put(v);
}

template <typename T, GLenum Target, GLenum Usage>
inline void Buffer<T, Target, Usage>::put(
    const std::vector<glm::tvec3<T> > value) {
        for (auto v : value) put(v);
}

template <typename T, GLenum Target, GLenum Usage>
inline void Buffer<T, Target, Usage>::bind() {
        bindFlag.check(false, "already was bound");
        glBindBuffer(Target, vbo);
        bindFlag.enable();
}

template <typename T, GLenum Target, GLenum Usage>
inline void Buffer<T, Target, Usage>::update() {
        bindFlag.check(true, "was not bound");
        if (!dirty) {
                return;
        }
        this->dirty = false;
        glBufferData(Target, sizeof(T) * vec.size(), vec.data(), Usage);
}

template <typename T, GLenum Target, GLenum Usage>
inline void Buffer<T, Target, Usage>::unbind() {
        bindFlag.check(true, "was not bound");
        glBindBuffer(Target, 0);
        bindFlag.disable();
}

template <GLenum Usage = GL_DYNAMIC_DRAW>
using VertexBuffer = Buffer<float, GL_ARRAY_BUFFER, Usage>;

template <GLenum Usage = GL_DYNAMIC_DRAW>
using NormalBuffer = Buffer<float, GL_ARRAY_BUFFER, Usage>;

template <GLenum Usage = GL_DYNAMIC_DRAW>
using UVBuffer = Buffer<float, GL_ARRAY_BUFFER, Usage>;

template <GLenum Usage = GL_DYNAMIC_DRAW>
using IndexBuffer = Buffer<GLushort, GL_ELEMENT_ARRAY_BUFFER, Usage>;
}  // namespace planet
#endif