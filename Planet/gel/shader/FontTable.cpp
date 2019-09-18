#include "FontTable.hpp"
#include "../asset/AssetDatabase.hpp"
#include "ShaderRegistry.hpp"

namespace gel {
FontTable::FontTable() : texMap() {}
void FontTable::init(const std::vector<std::string> lines, Rule rule) {
        int count = 0;
        int row = 0;
        int col = 0;
        for (std::string line : lines) {
                for (char c : line) {
                        auto tex = gel::AssetDatabase::getAsset<ITexture>(
                            rule(c, row, col));
                        gel::Sprite sprite(
                            gel::ShaderRegistry::getInstance().get("Texture2D"),
                            gel::NameRule());
                        sprite.init(tex->getID(), glm::vec2(0, 0),
                                    tex->getSize(), 1.0f);
                        texMap.insert_or_assign(c, sprite);
                        count++;
                        col++;
                }
                row++;
                col = 0;
        }
}
void FontTable::destroy() {
        for (auto e : texMap) {
                e.second.destroy();
        }
        texMap.clear();
}
void FontTable::draw(std::shared_ptr<Camera> camera, glm::vec2 pos, char c) {
        gel::Sprite& sprite = texMap.at(c);
        sprite.reshape(pos, sprite.getSize());
        sprite.draw(camera);
}
void FontTable::draw(std::shared_ptr<Camera> camera, glm::vec2 pos,
                     glm::vec2 interval, const std::string str) {
        glm::vec2 npos = pos;
        for (char c : str) {
                draw(camera, npos, c);
                npos += interval;
        }
}
void FontTable::draw(std::shared_ptr<Camera> camera, glm::vec2 pos, glm::vec2 interval, const char * str)
{
	draw(camera, pos, interval, std::string(str));
}
}  // namespace gel