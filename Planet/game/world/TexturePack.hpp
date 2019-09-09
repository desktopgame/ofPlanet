#ifndef GAME_WORLD_TEXTUREPACK_HPP
#define GAME_WORLD_TEXTUREPACK_HPP
#include <string>
#include "../../gel/gel.hpp"
class TexturePack {
       public:
        explicit TexturePack();
        static TexturePack side1(const std::string& path);
        static TexturePack side3(const std::string& prefix,
                                 const std::string& suffix);
        static TexturePack side6(const std::string& prefix,
                                 const std::string& suffix);
        GLuint posXTex;
        GLuint posYTex;
        GLuint posZTex;
        GLuint negXTex;
        GLuint negYTex;
        GLuint negZTex;
};
#endif