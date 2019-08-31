#include "Text.hpp"
namespace gel {
Text::Text() : position(), texMap(), cellSize(), text() {}

void Text::put(char c, const std::weak_ptr<ITexture>& tex) { texMap[c] = tex; }
void Text::remoe(char c, const std::weak_ptr<ITexture>& tex) {
        texMap.erase(c);
}

void Text::putAlpha(const std::string baseName,
                    const std::shared_ptr<TextureManager>& textureManager) {
        for (int i = 0; i < ('z' - 'a') + 1; i++) {
                char c = 'a' + i;
                auto path = baseName + "_" + c + ".png";
                put(c, textureManager->getTexture(path));
        }
}
void Text::putDigit(const std::string baseName,
                    const std::shared_ptr<TextureManager>& textureManager) {
        for (int i = 0; i < ('9' - '0') + 1; i++) {
                char c = '0' + i;
                auto path = baseName + "_" + c + ".png";
                put(c, textureManager->getTexture(path));
        }
}

void Text::draw() {
        auto cur = position;
        for (auto c : text) {
                if (texMap.count(c)) {
                        //        drawTexture(cur, gel::Color4::white(1),
                        //                    texMap[c].lock());
                }
                cur.x += cellSize.x;
        }
}
}  // namespace gel