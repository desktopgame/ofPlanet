#ifndef GEL_GSYSTEM_NUMBER_HPP
#define GEL_GSYSTEM_NUMBER_HPP
#include <array>
#include <functional>
#include <glm/glm.hpp>
#include <string>
namespace gel {
class TextureManager;
/**
 * Number is class of draw for number.
 */
class Number {
       public:
        /**
         * @param textureManager
         * @param files
         */
        explicit Number(const std::shared_ptr<TextureManager>& textureManager,
                        const std::array<std::string, 10>& files);
        /**
         * @param i
         */
        void draw(int i);
        /**
         * @param i
         * @return
         */
        glm::vec2 calcSize(int i) const;
        glm::vec2 position;

       private:
        std::shared_ptr<TextureManager> textureManager;
        std::array<std::string, 10> files;
        void drawTexture(const glm::vec2& pos, int index);
};
}  // namespace gel
#endif