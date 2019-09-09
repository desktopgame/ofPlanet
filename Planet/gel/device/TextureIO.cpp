#include "TextureIO.hpp"
#include "BmpTexture.hpp"
#include "JpegTexture.hpp"
#include "PngTexture.hpp"

namespace gel {
std::shared_ptr<ITexture> TextureIO::load(const std::string path) {
        auto ext = path.substr(path.find_last_of(".") + 1);
        std::shared_ptr<ITexture> ret = nullptr;
        if (ext == "bmp") {
                ret = std::make_shared<BmpTexture>();
        } else if (ext == "png") {
                ret = std::make_shared<PngTexture>();
        } else if (ext == "jpg" || ext == "jpeg") {
                ret = std::make_shared<JpegTexture>();
        }
        if (ret) {
                ret->loadOnAsync(path);
                ret->loadOnGLThread(path);
        }
        return ret;
}
}  // namespace gel