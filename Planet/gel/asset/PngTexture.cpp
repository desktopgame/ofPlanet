#include "PngTexture.hpp"
#include "AssetLoadException.hpp"

#if __APPLE__
#include <libpng16/png.h>
#else
#include <png.h>
#endif
#include <assert.h>
#include <stdlib.h>
#if _WIN32
#pragma warning(disable : 4996)
#endif
namespace gel {
PngTexture::PngTexture() : textureId(0), width(0), height(0), data(NULL) {}

void PngTexture::load(const std::string& path, Thread thread) {
        if (thread == Thread::OnBackground) {
                loadBackground(path, thread);
        } else if (thread == Thread::OnGL) {
                // generate texture
                glGenTextures(1, &(this->textureId));
                glBindTexture(GL_TEXTURE_2D, this->textureId);
                // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                // GL_NEAREST); glTexParameteri(GL_TEXTURE_2D,
                // GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                                GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                                GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                                GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
                glEnable(GL_TEXTURE_2D);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width,
                             this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             this->data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
        }
}

void PngTexture::unload() {
        glDeleteTextures(1, &(this->textureId));
        free(this->data);
}

GLuint PngTexture::getID() const { return textureId; }

unsigned char* PngTexture::getData() const { return data; }

int PngTexture::getWidth() const { return width; }

int PngTexture::getHeight() const { return height; }

glm::vec2 PngTexture::getSize() const { return glm::vec2(width, height); }

void PngTexture::loadBackground(const std::string& path, Thread thread) {
        FILE* fp = fopen(path.c_str(), "rb");
        // check file exists
        if (!fp) {
			throw AssetLoadException::createFromErrno();
        }
        png_structp pPng =
            png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop pInfo = png_create_info_struct(pPng);
        assert(pPng != NULL);
        assert(pInfo != NULL);
        png_init_io(pPng, fp);
        png_read_info(pPng, pInfo);
        png_get_IHDR(pPng, pInfo, &(this->width), &(this->height),
                     &(this->depth), &(this->colorType), &(this->interlaceType),
                     NULL, NULL);
        // check bounds
        if (this->width == 0 || this->height == 0) {
			throw AssetLoadException("LoadPng: Invalid Bounds ");
        }
        // check color type
        if (this->colorType != PNG_COLOR_TYPE_RGB &&
            this->colorType != PNG_COLOR_TYPE_RGBA) {
			throw AssetLoadException("LoadPng: Supported color type are RGB and RGBA.");
        }
        // check interlace type
        if (this->interlaceType != PNG_INTERLACE_NONE) {
			throw AssetLoadException("LoadPng: Interlace image is not supported.");
        }
        int rowSize = png_get_rowbytes(pPng, pInfo);
        // int imgSize = rowSize * this->height;
        int imgSize = rowSize * this->height;
        this->data = (unsigned char*)::malloc(imgSize);
        assert(this->data != NULL);
        // read pixels
        for (int i = 0; i < this->height; i++) {
                png_read_row(pPng, &(this->data[i * rowSize]), NULL);
        }
        png_read_end(pPng, pInfo);

        png_destroy_info_struct(pPng, &pInfo);
        png_destroy_read_struct(&pPng, NULL, NULL);
        fclose(fp);
}

}  // namespace gel