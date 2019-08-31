#include "FbxMaterial.hpp"
namespace gel {
FbxMaterial::FbxMaterial()
    : shader(),
      name(),
      type(FbxMaterialType::Texture),
      mainColor(1.0f, 0.0f, 0.0f, 1.0f),
      color(),
      shininess(),
      texture(),
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
}  // namespace gel