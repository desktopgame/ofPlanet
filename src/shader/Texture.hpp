#pragma once
#ifndef SHADER_TEXTURE_HPP
#define SHADER_TEXTURE_HPP
#include <ofTypes.h>

#include <string>

#include "Flag.hpp"
namespace planet {

class Texture {
       public:
        explicit Texture();
        ~Texture();
        void load(const std::string& path);

        void bind();
        void unbind();

        int getWidth() const;
        int getHeight() const;

        unsigned char* getData() const;
        GLuint getName() const;
		std::string getPath() const;

       private:
        Texture(const Texture& obj) = delete;
        Texture& operator=(const Texture&) = delete;
		std::string path;
        Flag loadFlag;
        Flag bindFlag;
        int width, height, ch;
        unsigned char* data;
        GLuint name;
};
}  // namespace planet
#endif