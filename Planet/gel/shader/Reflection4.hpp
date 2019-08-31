#ifndef GEL_SHADER_REFLECTION_HPP
#define GEL_SHADER_REFLECTION_HPP
#include "Color4.hpp"
namespace gel {
struct Reflection4 {
        Reflection4();
        Color4 diffuse;
        Color4 ambient;
        Color4 emission;
        Color4 specular;
};
}  // namespace  gel

#endif