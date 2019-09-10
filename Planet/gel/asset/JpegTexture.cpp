#include "JpegTexture.hpp"
#include <jpeglib.h>
// https://daeudaeu.com/programming/c-language/libjpeg/
namespace gel {
JpegTexture::JpegTexture() : textureId(0), width(0), height(0), data(NULL) {}
void JpegTexture::load(const std::string& path, Thread thread) {
        if (thread == Thread::OnBackground) {
                loadBackground(path, thread);
        } else if (thread == Thread::OnGL) {
                // generate texture
                glGenTextures(1, &(this->textureId));
                glBindTexture(GL_TEXTURE_2D, this->textureId);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR);
                glEnable(GL_TEXTURE_2D);
                glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB,
                             GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
        }
}
void JpegTexture::unload() {
        glDeleteTextures(1, &(this->textureId));
        std::free(data);
}
GLuint JpegTexture::getID() const { return textureId; }
unsigned char* JpegTexture::getData() const { return data; }
int JpegTexture::getWidth() const { return width; }
int JpegTexture::getHeight() const { return height; }
glm::vec2 JpegTexture::getSize() const { return glm::vec2(width, height); }

void JpegTexture::loadBackground(const std::string& path, Thread thread) {
        struct jpeg_decompress_struct jpeg;
        struct jpeg_error_mgr err;
        JSAMPLE* tmp;
        jpeg.err = jpeg_std_error(&err);
        FILE* fi = fopen(path.c_str(), "rb");
        if (fi == NULL) {
                perror("JpegTexture#load");
                return;
        }

        jpeg_create_decompress(&jpeg);
        jpeg_stdio_src(&jpeg, fi);
        jpeg_read_header(&jpeg, TRUE);
        jpeg_start_decompress(&jpeg);

        this->data = (unsigned char*)std::malloc(
            sizeof(unsigned char) * jpeg.output_width * jpeg.output_height *
            jpeg.out_color_components);
        if (this->data == NULL) {
                perror("JpegTexture#load");
                fclose(fi);
                jpeg_destroy_decompress(&jpeg);
                return;
        }

        for (int j = 0; j < jpeg.output_height; j++) {
                tmp = this->data +
                      j * jpeg.out_color_components * jpeg.output_width;
                jpeg_read_scanlines(&jpeg, &tmp, 1);
        }

        this->height = jpeg.output_height;
        this->width = jpeg.output_width;
        this->ch = jpeg.out_color_components;

        jpeg_finish_decompress(&jpeg);
        jpeg_destroy_decompress(&jpeg);
}
}  // namespace gel