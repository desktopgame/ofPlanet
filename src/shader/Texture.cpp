#include "Texture.hpp"

#include <soil.h>
namespace planet {

Texture::Texture()
    : loadFlag(false),
      bindFlag(false),
      width(-1),
      height(-1),
      ch(-1),
      data(nullptr),
      name(0) {
        glGenTextures(1, &name);
}

Texture::~Texture() {
        glDeleteTextures(1, &name);
        SOIL_free_image_data(data);
}

void Texture::load(const std::string& path) {
        loadFlag.check(false, "already was loaded");
        loadFlag.enable();
        this->path = path;
        this->data =
            SOIL_load_image(path.c_str(), &width, &height, &ch, SOIL_LOAD_RGBA);
        glBindTexture(GL_TEXTURE_2D, name);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() {
        bindFlag.check(false, "already was bound");
        glBindTexture(GL_TEXTURE_2D, name);
        bindFlag.enable();
}

void Texture::unbind() {
        bindFlag.check(true, "was not bound");
        glBindTexture(GL_TEXTURE_2D, 0);
        bindFlag.disable();
}

int Texture::getWidth() const { return width; }

int Texture::getHeight() const { return height; }

unsigned char* Texture::getData() const { return data; }

GLuint Texture::getName() const { return name; }
std::string Texture::getPath() const { return path; }
}  // namespace planet
