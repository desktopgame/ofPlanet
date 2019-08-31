#ifndef GEL_MODEL_FBXMATERIAL_HPP
#define GEL_MODEL_FBXMATERIAL_HPP
#include <string>
#include <vector>
#include "../gli.hpp"
#include "../math/Quadrangle.hpp"
#include "../math/Triangle.hpp"
#include "../shader/Buffer.hpp"
#include "../shader/Reflection4.hpp"
#include "../shader/VertexArray.hpp"
namespace gel {
enum class FbxMaterialType {
        Texture,
        Color,
};
struct FbxMaterial {
        FbxMaterial();

        std::string shader;
        std::string name;
        FbxMaterialType type;
        Color4 mainColor;

        Reflection4 color;
        float shininess;
        std::string texture;
        int textureNo;
        std::vector<Triangle> triangles;
        std::vector<Quadrangle> quads;
        Buffer<GLfloat> triVertex;
        Buffer<GLfloat> triUV;
        Buffer<GLfloat> triNormal;
        VertexArray triVAO;
        Buffer<GLfloat> quadVertex;
        Buffer<GLfloat> quadUV;
        Buffer<GLfloat> quadNormal;
        VertexArray quadVAO;
};
}  // namespace  gel
#endif