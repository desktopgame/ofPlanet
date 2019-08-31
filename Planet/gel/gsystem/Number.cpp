#include "Number.hpp"
#include "../device/ITexture.hpp"
#include "../device/TextureManager.hpp"

namespace gel {
Number::Number(const std::shared_ptr<TextureManager>& textureManager,
               const std::array<std::string, 10>& files)
    : textureManager(textureManager), files(files) {}
void Number::draw(int i) {
        glPushMatrix();
        if (i <= 9) {
                this->drawTexture(position, i);
        } else {
                auto str = std::to_string(i);
                auto temp = position;
                for (auto c : str) {
                        i = (int)(c - '0');
                        auto tex = textureManager->getTexture(files[i]);
                        this->drawTexture(temp, i);
                        temp.x += (tex->getWidth());
                }
        }
        glPopMatrix();
}
glm::vec2 Number::calcSize(int i) const {
        if (i <= 9) {
                auto tex = textureManager->getTexture(files[i]);
                return tex->getSize();
        } else {
                glm::vec2 ret(0, 0);
                ret.y = -1;
                auto str = std::to_string(i);
                auto temp = position;
                for (auto c : str) {
                        i = (int)(c - '0');
                        auto tex = textureManager->getTexture(files[i]);
                        if (tex->getHeight() > ret.y) {
                                ret.y = tex->getHeight();
                        }
                        ret.x += tex->getWidth();
                }
                return ret;
        }
}
void Number::drawTexture(const glm::vec2& pos, int index) {
        auto tex = textureManager->getTexture(files[index]);
        //::gel::drawTexture(pos, Color4::white(1), tex);
}
}  // namespace gel