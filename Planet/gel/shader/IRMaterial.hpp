#pragma once
#ifndef GEL_SHADER_IRMATERIAL_HPP
#define GEL_SHADER_IRMATERIAL_HPP
#include <string>
#include "../math/Quadrangle.hpp"
#include "../math/Triangle.hpp"
#include "Buffer.hpp"
#include "Color4.hpp"
#include "NameRule.hpp"
#include "Reflection4.hpp"
#include "VertexArray.hpp"

namespace gel {
class IRMesh;
enum class IRMaterialType {
        Color,
        Texture,
};

class IRMaterial {
       public:
        explicit IRMaterial();
        ~IRMaterial();

        void init();
        void update();
        void destroy();

        void setType(const IRMaterialType type);
        IRMaterialType getType() const;

        void setShader(const std::string& shader);
        std::string getShader() const;

        void setAmbient(const Color4& ambient);
        Color4 getAmbient() const;

        void setDiffuse(const Color4& diffuse);
        Color4 getDiffuse() const;

        void setSpecular(const Color4& specular);
        Color4 getSpecular() const;

        void setShininess(const float shininess);
        float getShininess() const;

        void setTexture(const int textureNo);
        int getTexture() const;

        void setMainColor(const Color4& mainColor);
        Color4 getMainColor() const;

        void draw(std::shared_ptr<IRMesh>& mesh, const NameRule& nameRule);

        void addTriangle(const Triangle& tria);
        void addQuad(const Quadrangle& quad);

       private:
        void applyTriangleVertex(const NameRule& nameRule);
        void applyQuadVertex(const NameRule& nameRule);

        IRMaterialType type;
        std::string shader;
        Color4 ambient;
        Color4 diffuse;
        Color4 specular;
        float shininess;
        int textureNo;
        Color4 mainColor;
        std::vector<Triangle> triangles;
        std::vector<Quadrangle> quads;

        std::string name;
        Buffer<GLfloat> triVertex;
        Buffer<GLfloat> triUV;
        Buffer<GLfloat> triNormal;
        VertexArray triVAO;
        Buffer<GLfloat> quadVertex;
        Buffer<GLfloat> quadUV;
        Buffer<GLfloat> quadNormal;
        VertexArray quadVAO;
};
}  // namespace gel
#endif