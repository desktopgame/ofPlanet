#include "CubeMap.hpp"
#include <stdexcept>
#include "../asset/AssetDatabase.hpp"
#include "../asset/ITexture.hpp"
namespace gel {
CubeMap::CubeMap(const std::shared_ptr<Shader>& shader,
                 const NameRule& nameRule)
    : shader(shader),
      nameRule(nameRule),
      vao(),
      vertex(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW) {}

void CubeMap::init(const CubeMapDesc& desc, const glm::vec3 scale,
                   const int width, const int height) {
        vao.init();
        vertex.init();
        // emit
        auto& vSeq = vertex.modify();
        vSeq.emitVec4(-scale.x, scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, -scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(scale.x, -scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(scale.x, -scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(scale.x, scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, scale.y, -scale.z, 1.0f);

        vSeq.emitVec4(-scale.x, -scale.y, scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, -scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, scale.y, scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, -scale.y, scale.z, 1.0f);

        vSeq.emitVec4(scale.x, -scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(scale.x, -scale.y, scale.z, 1.0f);
        vSeq.emitVec4(scale.x, scale.y, scale.z, 1.0f);
        vSeq.emitVec4(scale.x, scale.y, scale.z, 1.0f);
        vSeq.emitVec4(scale.x, scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(scale.x, -scale.y, -scale.z, 1.0f);

        vSeq.emitVec4(-scale.x, -scale.y, scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, scale.y, scale.z, 1.0f);
        vSeq.emitVec4(scale.x, scale.y, scale.z, 1.0f);
        vSeq.emitVec4(scale.x, scale.y, scale.z, 1.0f);
        vSeq.emitVec4(scale.x, -scale.y, scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, -scale.y, scale.z, 1.0f);

        vSeq.emitVec4(-scale.x, scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(scale.x, scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(scale.x, scale.y, scale.z, 1.0f);
        vSeq.emitVec4(scale.x, scale.y, scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, scale.y, scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, scale.y, -scale.z, 1.0f);

        vSeq.emitVec4(-scale.x, -scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, -scale.y, scale.z, 1.0f);
        vSeq.emitVec4(scale.x, -scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(scale.x, -scale.y, -scale.z, 1.0f);
        vSeq.emitVec4(-scale.x, -scale.y, scale.z, 1.0f);
        vSeq.emitVec4(scale.x, -scale.y, scale.z, 1.0f);

        vertex.bind();
        vertex.update();
        vertex.unbind();
        // bind
        GLuint vertexAttrib = shader->getAttribLocation(nameRule.attribVertex);
        shader->use();
        shader->setUniform1i("skybox", 0);
        vao.bind();
        vertex.bind();
        glVertexAttribPointer(vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexAttrib);
        vao.unbind();
        vertex.unbind();
        shader->unuse();
        // load texture
        this->texture = loadCubeMap(desc, width, height);
}
void CubeMap::destroy() {
        vao.destroy();
        vertex.destroy();
        glDeleteTextures(1, &texture);
}

void CubeMap::draw() {
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_TEXTURE_CUBE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_GEN_R);

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

        glDisable(GL_ALPHA_TEST);
        glDisable(GL_TEXTURE_CUBE_MAP);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_R);
}

// private
GLuint CubeMap::loadCubeMap(const CubeMapDesc& desc, const int width,
                            const int height) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        const unsigned char* front =
            AssetDatabase::getAsset<ITexture>(desc.front)->getData();
        const unsigned char* back =
            AssetDatabase::getAsset<ITexture>(desc.back)->getData();
        const unsigned char* left =
            AssetDatabase::getAsset<ITexture>(desc.left)->getData();
        const unsigned char* right =
            AssetDatabase::getAsset<ITexture>(desc.right)->getData();
        const unsigned char* top =
            AssetDatabase::getAsset<ITexture>(desc.top)->getData();
        const unsigned char* bottom =
            AssetDatabase::getAsset<ITexture>(desc.bottom)->getData();

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
}  // namespace gel