#include "StatusUI.hpp"
#include "../../gel/asset/AssetDatabase.hpp"
#include "../../gel/shader/ShaderRegistry.hpp"
#include "StatusModel.hpp"

StatusUI::StatusUI()
    : bulletSprite(gel::ShaderRegistry::getInstance().get("Texture2D"),
                   gel::NameRule()),
      hartSprite(gel::ShaderRegistry::getInstance().get("Texture2D"),
                 gel::NameRule()),
      fontTable(),
      model(std::make_shared<StatusModel>(100,128)){}
void StatusUI::init() {
        hartSprite.init(gel::AssetDatabase::getAsset<gel::ITexture>(
                            "./assets/image/hart_gray.png")
                            ->getID(),
                        glm::vec2(0, 0), glm::vec2(64, 64), 1.0f);
        bulletSprite.init(gel::AssetDatabase::getAsset<gel::ITexture>(
                              "./assets/image/bullet_gray.png")
                              ->getID(),
                          glm::vec2(0, 64), glm::vec2(64, 64), 1.0f);
        fontTable.init(
			std::vector<std::string>{"abcdefghijklmn",
			"opqrstuvwxyz.:", "0123456789()", "ABCDEFGHIJKLMN", "OPQRSTUVWXYZ+-"},
            [](char c, int row, int col) -> std::string {
                    std::string rs = std::to_string(row);
                    std::string cs = std::to_string(col);
                    return "./assets/image/font/ascii/FontTable64_" + rs + "_" +
                           cs + ".png";
            });
}
void StatusUI::destroy() {
        hartSprite.destroy();
        bulletSprite.destroy();
        fontTable.destroy();
}
void StatusUI::draw(std::shared_ptr<gel::Camera> camera) {
        hartSprite.draw(camera);
        bulletSprite.draw(camera);
        fontTable.draw(camera, glm::vec2(64, 0), glm::vec2(38, 0), std::to_string(model->getHP()));
        fontTable.draw(camera, glm::vec2(64, 64), glm::vec2(38, 0), std::to_string(model->getAmmo()));
}

std::shared_ptr<StatusModel> StatusUI::getModel() const
{
	return model;
}
