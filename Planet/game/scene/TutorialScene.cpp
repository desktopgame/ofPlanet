#include "TutorialScene.hpp"
#include "../../gel/device/AssetDatabase.hpp"

TutorialScene::TutorialScene(const std::shared_ptr<gel::GameDevice>& gameDevice)
    : sprites(),
      page(0),
      finished(false),
      gameDevice(gameDevice),
      camera(std::make_shared<gel::Camera>()),
      mouseTrigger(GLFW_MOUSE_BUTTON_LEFT) {
        std::vector<std::string> spritePaths = std::vector<std::string>{
            "./assets/image/tutorial/tutorial_control.png",
            "./assets/image/tutorial/tutorial_hill.png",
            "./assets/image/tutorial/tutorial_desert.png",
            "./assets/image/tutorial/tutorial_warp.png",
            "./assets/image/tutorial/tutorial_wait.png",
        };
        for (auto path : spritePaths) {
                gel::Sprite sprite(
                    gel::ShaderRegistry::getInstance().get("Texture2D"));
                sprite.init(
                    gel::AssetDatabase::getAsset<gel::ITexture>(path)->getID(),
                    glm::vec2(), glm::vec2(1280, 720), 1);
                sprites.push_back(sprite);
        }
        auto wsize = gel::Game::getInstance()->getWindowSize();
        camera->screenWidth = wsize.x;
        camera->screenHeight = wsize.y;
        camera->calculate();
}

TutorialScene::~TutorialScene() {}

void TutorialScene::show() {
        this->elapsed = 0;
        this->finished = false;
        this->page = 0;
        this->wait = 0;
        this->waitStart = false;
        glfwSetInputMode(gel::Game::getInstance()->getWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void TutorialScene::update() {
        float delta = gel::Game::getInstance()->getDeltaTime();
        this->elapsed += delta;
        if (elapsed < 0.9f) {
                return;
        }
        mouseTrigger.update();
        if (mouseTrigger.isEnabled()) {
                page += 1;
        }
        if (!waitStart) {
                int n = (page + 1);
                if (n >= static_cast<int>(sprites.size())) {
                        this->waitStart = true;
                }
        } else {
                wait += delta;
                if (wait > 1.0f) {
                        this->finished = true;
                }
        }
}

void TutorialScene::draw() {
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int tmp = page;
        if (tmp >= sprites.size()) {
                tmp = sprites.size() - 1;
        }
        sprites[tmp].draw(camera);
}

void TutorialScene::hide() {
        this->finished = false;
        this->page = 0;
}

std::string TutorialScene::getNextScene() const { return "play"; }

bool TutorialScene::isFinished() const { return finished; }
