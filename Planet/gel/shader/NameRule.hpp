#ifndef GEL_SHADER_NAMERULE_HPP
#define GEL_SHADER_NAMERULE_HPP
#include <string>
namespace gel {
/**
 * NameRule is rule of naming rule for shader.
 */
class NameRule {
       public:
        explicit NameRule();
        /**
         * set default values.
         */
        void defaults();

        std::string attribVertex;
        std::string attribNormal;
        std::string attribColor;
        std::string attribUV;
        std::string attribPosition;
        std::string uniformModelMatrix;
        std::string uniformViewMatrix;
        std::string uniformProjectionMatrix;
        std::string uniformMVPMatrix;
        std::string uniformNormalMatrix;
        std::string uniformTexture;
        std::string uniformAlpha;
        std::string uniformColor;
        std::string uniformPosition;
        std::string uniformAmbient;
        std::string uniformDiffuse;
        std::string uniformSpecular;
        std::string uniformShininess;

       private:
};
}  // namespace gel
#endif