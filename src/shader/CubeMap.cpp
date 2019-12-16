#include "CubeMap.hpp"

#include <ofApp.h>
#include <soil.h>

#include "../common/GLM.hpp"

#include "Camera.hpp"
#include "NameSet.hpp"
#include "Shader.hpp"
namespace planet {

CubeMap::CubeMap(const CubeMapDesc& desc, const NameSet& nameSet,
                 const glm::vec3 scale, int width, const int height)
    : texture(0), nameSet(nameSet), vertex(), scale(scale) {
        vertex.put(glm::vec3(-scale.x, scale.y, -scale.z));
        vertex.put(glm::vec3(-scale.x, -scale.y, -scale.z));
        vertex.put(glm::vec3(scale.x, -scale.y, -scale.z));
        vertex.put(glm::vec3(scale.x, -scale.y, -scale.z));
        vertex.put(glm::vec3(scale.x, scale.y, -scale.z));
        vertex.put(glm::vec3(-scale.x, scale.y, -scale.z));

        vertex.put(glm::vec3(-scale.x, -scale.y, scale.z));
        vertex.put(glm::vec3(-scale.x, -scale.y, -scale.z));
        vertex.put(glm::vec3(-scale.x, scale.y, -scale.z));
        vertex.put(glm::vec3(-scale.x, scale.y, -scale.z));
        vertex.put(glm::vec3(-scale.x, scale.y, scale.z));
        vertex.put(glm::vec3(-scale.x, -scale.y, scale.z));

        vertex.put(glm::vec3(scale.x, -scale.y, -scale.z));
        vertex.put(glm::vec3(scale.x, -scale.y, scale.z));
        vertex.put(glm::vec3(scale.x, scale.y, scale.z));
        vertex.put(glm::vec3(scale.x, scale.y, scale.z));
        vertex.put(glm::vec3(scale.x, scale.y, -scale.z));
        vertex.put(glm::vec3(scale.x, -scale.y, -scale.z));

        vertex.put(glm::vec3(-scale.x, -scale.y, scale.z));
        vertex.put(glm::vec3(-scale.x, scale.y, scale.z));
        vertex.put(glm::vec3(scale.x, scale.y, scale.z));
        vertex.put(glm::vec3(scale.x, scale.y, scale.z));
        vertex.put(glm::vec3(scale.x, -scale.y, scale.z));
        vertex.put(glm::vec3(-scale.x, -scale.y, scale.z));

        vertex.put(glm::vec3(-scale.x, scale.y, -scale.z));
        vertex.put(glm::vec3(scale.x, scale.y, -scale.z));
        vertex.put(glm::vec3(scale.x, scale.y, scale.z));
        vertex.put(glm::vec3(scale.x, scale.y, scale.z));
        vertex.put(glm::vec3(-scale.x, scale.y, scale.z));
        vertex.put(glm::vec3(-scale.x, scale.y, -scale.z));

        vertex.put(glm::vec3(-scale.x, -scale.y, -scale.z));
        vertex.put(glm::vec3(-scale.x, -scale.y, scale.z));
        vertex.put(glm::vec3(scale.x, -scale.y, -scale.z));
        vertex.put(glm::vec3(scale.x, -scale.y, -scale.z));
        vertex.put(glm::vec3(-scale.x, -scale.y, scale.z));
        vertex.put(glm::vec3(scale.x, -scale.y, scale.z));

        auto shader = nameSet.getShader();
        vertex.bind();
        vertex.update();
        vertex.unbind();
        // bind
        shader->use();
        assert(nameSet.aVertex.enabled);
        GLuint vertexAttrib = shader->getAttribLocation(nameSet.aVertex.name);
        assert(nameSet.uSkybox.enabled);
        shader->setUniform1i(nameSet.uSkybox.name, 0);
        vao.bind();
        vertex.bind();
        glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexAttrib);
        vao.unbind();
        vertex.unbind();
        shader->unuse();
        // load texture
        this->texture = loadCubeMap(desc, width, height);
}

CubeMap::~CubeMap() { glDeleteTextures(1, &texture); }

void CubeMap::draw() {
        auto shader = nameSet.getShader();
        shader->use();
        vao.bind();
        glDepthFunc(GL_LEQUAL);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glDepthFunc(GL_LESS);
        vao.unbind();
        shader->unuse();
}

void CubeMap::onRehash(std::shared_ptr<const Camera> camera) {
        auto shader = nameSet.getShader();
        auto mat = glm::mat4(1.0f);
        mat = glm::translate(mat, scale / 2);
        shader->use();
        shader->setUniformMatrix4f(nameSet.uModelMatrix.name, 1, GL_FALSE,
                                   (mat));
        shader->setUniformMatrix4f(nameSet.uProjectionMatrix.name, 1, GL_FALSE,
                                   (camera->getProjectionMatrix()));
        shader->setUniformMatrix4f(nameSet.uViewMatrix.name, 1, GL_FALSE,
                                   (camera->getViewMatrix()));
        shader->unuse();
}

// private
GLuint CubeMap::loadCubeMap(const CubeMapDesc& desc, const int width,
                            const int height) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        unsigned char* front = desc.front;
        unsigned char* back = desc.back;
        unsigned char* left = desc.left;
        unsigned char* right = desc.right;
        unsigned char* top = desc.top;
        unsigned char* bottom = desc.bottom;

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, right);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, left);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, top);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, bottom);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, front);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, back);
        return textureID;
}

CubeMapDesc::CubeMapDesc()
    : right(nullptr),
      left(nullptr),
      top(nullptr),
      bottom(nullptr),
      front(nullptr),
      back(nullptr) {}
}  // namespace planet
