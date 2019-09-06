#include "IRMaterial.hpp"
#include "../shader/Shader.hpp"
#include "../shader/ShaderRegistry.hpp"
#include "IRMesh.hpp"
namespace gel {
IRMaterial::IRMaterial()
    : shader(),
      name(),
      type(IRMaterialType::Texture),
      mainColor(1.0f, 1.0f, 1.0f, 1.0f),
      ambient(),
      diffuse(),
      specular(),
      shininess(),
      textureNo(),
      triangles(),
      quads(),
      triVertex(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      triUV(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      triNormal(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      quadVertex(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      quadUV(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      quadNormal(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      triVAO(),
      quadVAO() {}

IRMaterial::~IRMaterial() {}

void IRMaterial::init() {
        triVAO.init();
        triVertex.init();
        triUV.init();
        triNormal.init();
        quadVAO.init();
        quadVertex.init();
        quadUV.init();
        quadNormal.init();
}

void IRMaterial::update() {
        triVertex.bind();
        triVertex.update();
        triVertex.unbind();

        triUV.bind();
        triUV.update();
        triUV.unbind();

        triNormal.bind();
        triNormal.update();
        triNormal.unbind();

        quadVertex.bind();
        quadVertex.update();
        quadVertex.unbind();

        quadUV.bind();
        quadUV.update();
        quadUV.unbind();

        quadNormal.bind();
        quadNormal.update();
        quadNormal.unbind();
}

void IRMaterial::destroy() {
        triVAO.destroy();
        triVertex.destroy();
        triUV.destroy();
        triNormal.destroy();
        quadVAO.destroy();
        quadVertex.destroy();
        quadUV.destroy();
        quadNormal.destroy();
}

void IRMaterial::setType(const IRMaterialType type) { this->type = type; }

IRMaterialType IRMaterial::getType() const { return type; }
void IRMaterial::setShader(const std::string& shader) { this->shader = shader; }
std::string IRMaterial::getShader() const { return shader; }
void IRMaterial::setAmbient(const Color4& ambient) { this->ambient = ambient; }
Color4 IRMaterial::getAmbient() const { return ambient; }
void IRMaterial::setDiffuse(const Color4& diffuse) { this->diffuse = diffuse; }
Color4 IRMaterial::getDiffuse() const { return diffuse; }
void IRMaterial::setSpecular(const Color4& specular) {
        this->specular = specular;
}
Color4 IRMaterial::getSpecular() const { return specular; }
void IRMaterial::setShininess(const float shininess) {
        this->shininess = shininess;
}
float IRMaterial::getShininess() const { return shininess; }
void IRMaterial::setTexture(const int textureNo) {
        this->textureNo = textureNo;
}
int IRMaterial::getTexture() const { return textureNo; }
void IRMaterial::setMainColor(const Color4& mainColor) {
        this->mainColor = mainColor;
}
Color4 IRMaterial::getMainColor() const { return mainColor; }

void IRMaterial::draw(std::shared_ptr<IRMesh>& mesh, const NameRule& nameRule) {
        if (mesh->getName() == "Cube") {
                return;
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        auto& shader = ShaderRegistry::getInstance().get(getShader());
        shader.use();
        if (textureNo > 0) {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,
                              mesh->getTexture(textureNo - 1)->getID());
        } else {
                glDisable(GL_TEXTURE_2D);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                if (getType() == IRMaterialType::Color) {
                        Color4 color = getMainColor();
                        shader.setUniform4f(nameRule.uniformColor, color.r,
                                            color.g, color.b, color.a);
                }
        }
        if (!triangles.empty()) {
                applyTriangleVertex(nameRule);
                triVAO.bind();
                glDrawArrays(GL_TRIANGLES, 0, triangles.size());
                triVAO.unbind();
        }
        if (!quads.empty()) {
                applyQuadVertex(nameRule);
                quadVAO.bind();
                glDrawArrays(GL_QUADS, 0, quads.size());
                quadVAO.unbind();
        }
        shader.unuse();
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
}

void IRMaterial::addTriangle(const Triangle& tria) {
        gel::Sequence<float>& vseq = triVertex.modify();
        gel::Sequence<float>& nseq = triNormal.modify();
        gel::Sequence<float>& useq = triUV.modify();
        vseq.emitVec4(tria.ver.x, tria.ver.y, tria.ver.z, tria.ver.w);
        nseq.emitVec4(tria.nor.x, tria.nor.y, tria.nor.z, tria.nor.w);
        useq.emitVec2(tria.uv.x, tria.uv.y);
        triangles.push_back(tria);
}
void IRMaterial::addQuad(const Quadrangle& quad)

{
        gel::Sequence<float>& vseq = quadVertex.modify();
        gel::Sequence<float>& nseq = quadNormal.modify();
        gel::Sequence<float>& useq = quadUV.modify();
        vseq.emitVec4(quad.ver.x, quad.ver.y, quad.ver.z, quad.ver.w);
        nseq.emitVec4(quad.nor.x, quad.nor.y, quad.nor.z, quad.nor.w);
        useq.emitVec2(quad.uv.x, quad.uv.y);
        quads.push_back(quad);
}
// private

void IRMaterial::applyTriangleVertex(const NameRule& nameRule) {
        if (triangles.empty()) {
                return;
        }
        Shader& shader = ShaderRegistry::getInstance().get(getShader());
        GLuint vertexAttrib = shader.getAttribLocation(nameRule.attribVertex);
        GLuint uvAttrib = shader.getAttribLocation(nameRule.attribUV);
        GLuint normalAttrib = shader.getAttribLocation(nameRule.attribNormal);
        triVAO.bind();
        triVertex.bind();
        glVertexAttribPointer(vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexAttrib);
        triNormal.bind();
        glVertexAttribPointer(normalAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(normalAttrib);
        if (getType() == IRMaterialType::Texture) {
                triUV.bind();
                glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
                glEnableVertexAttribArray(uvAttrib);
        } else {
                shader.setUniform4f(nameRule.uniformColor, mainColor.r,
                                    mainColor.g, mainColor.b, mainColor.a);
        }
        shader.setUniform1f(nameRule.uniformShininess, shininess);
        shader.setUniform4f(nameRule.uniformAmbient, ambient.r, ambient.g,
                            ambient.b, ambient.a);
        shader.setUniform4f(nameRule.uniformDiffuse, diffuse.r, diffuse.g,
                            diffuse.b, diffuse.a);
        shader.setUniform4f(nameRule.uniformSpecular, specular.r, specular.g,
                            specular.b, specular.a);
        triVAO.unbind();
        triVertex.unbind();
        triNormal.unbind();
        if (getType() == IRMaterialType::Texture) {
                triUV.unbind();
        }
}
void IRMaterial::applyQuadVertex(const NameRule& nameRule) {
        if (quads.empty()) {
                return;
        }
        Shader& shader = ShaderRegistry::getInstance().get(getShader());
        GLuint vertexAttrib = shader.getAttribLocation(nameRule.attribVertex);
        GLuint uvAttrib = shader.getAttribLocation(nameRule.attribUV);
        GLuint normalAttrib = shader.getAttribLocation(nameRule.attribNormal);
        quadVAO.bind();
        quadVertex.bind();
        glVertexAttribPointer(vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexAttrib);
        quadNormal.bind();
        glVertexAttribPointer(normalAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(normalAttrib);
        if (getType() == IRMaterialType::Texture) {
                quadUV.bind();
                glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
                glEnableVertexAttribArray(uvAttrib);
        } else {
                shader.setUniform4f(nameRule.uniformColor, mainColor.r,
                                    mainColor.g, mainColor.b, mainColor.a);
        }
        shader.setUniform1f(nameRule.uniformShininess, shininess);
        shader.setUniform4f(nameRule.uniformAmbient, ambient.r, ambient.g,
                            ambient.b, ambient.a);
        shader.setUniform4f(nameRule.uniformDiffuse, diffuse.r, diffuse.g,
                            diffuse.b, diffuse.a);
        shader.setUniform4f(nameRule.uniformSpecular, specular.r, specular.g,
                            specular.b, specular.a);
        quadVAO.unbind();
        quadVertex.unbind();
        quadNormal.unbind();
        if (getType() == IRMaterialType::Texture) {
                quadUV.unbind();
        }
}
}  // namespace gel