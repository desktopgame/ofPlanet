#ifndef GEL_SHADER_COLOR4_HPP
#define GEL_SHADER_COLOR4_HPP
namespace gel {
struct Color4 {
        Color4();
        Color4(float r, float g, float b, float a);
        float r;
        float g;
        float b;
        float a;
        static Color4 white(float a);
        static Color4 black(float a);
};
}  // namespace gel

#endif