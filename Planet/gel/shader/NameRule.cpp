#include "NameRule.hpp"
namespace gel {
NameRule::NameRule() { defaults(); }

void NameRule::defaults() {
        this->attribVertex = "aVertex";
        this->attribNormal = "aNormal";
        this->attribColor = "aColor";
        this->attribPosition = "aPosition";
        this->attribUV = "aUV";
        this->uniformModelMatrix = "uModelMatrix";
        this->uniformViewMatrix = "uViewMatrix";
        this->uniformProjectionMatrix = "uProjectionMatrix";
        this->uniformMVPMatrix = "uMVPMatrix";
        this->uniformTexture = "uTexture";
        this->uniformAlpha = "uAlpha";
        this->uniformColor = "uColor";
        this->uniformPosition = "uPosition";
        this->uniformAmbient = "uAmbient";
        this->uniformDiffuse = "uDiffuse";
        this->uniformSpecular = "uSpecular";
        this->uniformShininess = "uShininess";
}
}  // namespace gel