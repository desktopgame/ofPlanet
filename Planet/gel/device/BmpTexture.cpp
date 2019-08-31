#include "BmpTexture.hpp"
#if _WIN32
#pragma warning(disable : 4996)
#endif
namespace gel {
BmpTexture::BmpTexture() : tid(0), width(0), height(0) {}

void BmpTexture::load(const std::string& path) {
        readBMP(path.c_str());
        // generate texture
        glGenTextures(1, &(this->tid));
        glBindTexture(GL_TEXTURE_2D, this->tid);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, this->width, this->height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, this->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void BmpTexture::unload(const std::string& path) {
        glDeleteTextures(1, &(this->tid));
        delete[] data;
}

GLuint BmpTexture::getID() const { return this->tid; }

unsigned char* BmpTexture::getData() const { return data; }

int BmpTexture::getWidth() const { return this->width; }

int BmpTexture::getHeight() const { return this->height; }

glm::vec2 BmpTexture::getSize() const { return glm::vec2(width, height); }

// private
void BmpTexture::readBMP(const char* filename) {
        int i;
        FILE* f = fopen(filename, "rb");
        unsigned char info[54];
        fread(info, sizeof(unsigned char), 54, f);  // read the 54-byte header

        // extract image height and width from header
        this->width = *(int*)&info[18];
        this->height = *(int*)&info[22];

        int size = 3 * width * height;
        this->data = new unsigned char[size];  // allocate 3 bytes per pixel
        fread(data, sizeof(unsigned char), size,
              f);  // read the rest of the data at once
        fclose(f);

        for (i = 0; i < size; i += 3) {
                unsigned char tmp = data[i];
                data[i] = data[i + 2];
                data[i + 2] = tmp;
        }
}
}  // namespace gel