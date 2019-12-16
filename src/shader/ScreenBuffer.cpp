#include "ScreenBuffer.hpp"

#include <ofVec3f.h>

#include <vector>

#include "Shader.hpp"
namespace planet {

ScreenBuffer::ScreenBuffer(const NameSet& nameSet, int width, int height)
    : nameSet(nameSet),
      width(width),
      height(height),
      texture(0),
      frameBuffer(),
      renderBuffer(),
      shader(nameSet.getShader()),
      clearColor(),
      vao() {
        initRect();
        // init frame buffer
        frameBuffer.bind();

        // gen texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, texture, 0);

        // init render buffer
        renderBuffer.bind();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,
                              height);
        renderBuffer.target(GL_DEPTH_STENCIL_ATTACHMENT);
        // renderBuffer.unbind();

        frameBuffer.unbind();
        renderBuffer.unbind();
        glBindTexture(GL_TEXTURE_2D, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE) {
                throw std::logic_error("invalid state");
        }
}

ScreenBuffer::~ScreenBuffer() {}

void ScreenBuffer::bind() {
        frameBuffer.bind();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ScreenBuffer::unbind() { frameBuffer.unbind(); }

void ScreenBuffer::render() {
        glDisable(GL_DEPTH_TEST);
        // glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
        // GL_ONE_MINUS_SRC_ALPHA);
        shader->use();
        glBindTexture(GL_TEXTURE_2D, texture);
        vao.drawElements(GL_TRIANGLES, 6);
        shader->unuse();
        glEnable(GL_DEPTH_TEST);
}

void ScreenBuffer::initRect() {
        glm::vec3 size(1);
        std::vector<glm::vec3> verts = std::vector<glm::vec3>{
            glm::vec3(-size.x, -size.y, size.z),
            glm::vec3(size.x, -size.y, size.z),
            glm::vec3(size.x, size.y, size.z),
            glm::vec3(-size.x, size.y, size.z),
        };
        std::vector<glm::vec2> texcoords = std::vector<glm::vec2>{
            glm::vec2(0, 1),
            glm::vec2(1, 1),
            glm::vec2(1, 0),
            glm::vec2(0, 0),
        };
        std::vector<ofIndexType> indexData =
            std::vector<ofIndexType>{0, 1, 2, 2, 3, 0};
        auto shader = nameSet.getShader();
        shader->use();
        vao.bind();
        vao.setVertexData(verts.data(), verts.size(), GL_STATIC_DRAW);
        vao.setTexCoordData(texcoords.data(), texcoords.size(), GL_STATIC_DRAW);
        vao.setIndexData(indexData.data(), indexData.size(), GL_STATIC_DRAW);
        vao.unbind();

        shader->setUniform1i(nameSet.uTexture.name, 0);
        shader->unuse();
}
}  // namespace planet
