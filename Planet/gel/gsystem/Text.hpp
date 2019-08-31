#ifndef GEL_GSYSTEM_TEXT_HPP
#define GEL_GSYSTEM_TEXT_HPP
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include "../device/ITexture.hpp"
#include "../device/TextureManager.hpp"
namespace gel {
class Text {
       public:
        explicit Text();
        void put(char c, const std::weak_ptr<ITexture>& tex);
        void remoe(char c, const std::weak_ptr<ITexture>& tex);

        void putAlpha(const std::string baseName,
                      const std::shared_ptr<TextureManager>& textureManager);
        void putDigit(const std::string baseName,
                      const std::shared_ptr<TextureManager>& textureManager);
        void draw();
        glm::vec2 cellSize;
        glm::vec2 position;
        std::string text;

       private:
        std::unordered_map<char, std::weak_ptr<ITexture> > texMap;
};
}  // namespace gel
#endif