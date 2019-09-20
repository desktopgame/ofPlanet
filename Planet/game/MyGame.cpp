#include "MyGame.hpp"
#include "scene/LoadScene.hpp"
#include "scene/PlayScene.hpp"
#include "scene/TestScene.hpp"
#include "scene/TitleScene.hpp"
#include "scene/TutorialScene.hpp"
MyGame::MyGame() : sceneManager(), isExitNow(false) {}

void MyGame::onInit() {
        this->outputDebugMessage = false;
        // init shader
        gel::ShaderRegistry::put("Texture2D", "./texture2D.vert", "./texture2D.frag");
        gel::ShaderRegistry::put("Texture3D", "./texture.vert", "./texture.frag");
        gel::ShaderRegistry::put("TextureFixed", "./textureFixed.vert", "./textureFixed.frag");
        gel::ShaderRegistry::put("ColorFixed", "./colorFixed.vert", "./colorFixed.frag");
        gel::ShaderRegistry::put("Screen", "./screen.vert", "./screen.frag");
        gel::ShaderRegistry::put("SkyBox", "./skybox.vert", "./skybox.frag");
        gel::ShaderRegistry::put("Color", "./color.vert", "./color.frag");
        gel::ShaderRegistry::put("Noise", "./noise.vert", "./noise.frag");
        gel::ShaderRegistry::put("Gun", "./noise.vert", "./noise.frag");
        gel::ShaderRegistry::put("CRT", "./crt.vert", "./crt.frag");
        GLfloat lineWidthRange[2] = {0.0f, 0.0f};
        glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
        // Anisotropic filtering
        float filter;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &filter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, filter);
        getContentManager()->loadFully([](const std::string& file) -> bool {
                bool isFT = (file.find("./assets/image/font/ascii") !=
                             std::string::npos);
                bool isProg =
                    (file.find("./assets/image/Progress") != std::string::npos);
                return isFT || isProg;
        });
        sceneManager.put("load", std::make_shared<LoadScene>());
        sceneManager.bind("load");
}

void MyGame::onLoad() {
        getContentManager()->load(Thread::OnBackground);
        std::this_thread::sleep_for(std::chrono::seconds(1));
}

void MyGame::onStart() {
        getContentManager()->load(Thread::OnGL);
        // init block
        BlockRegistry& reg = BlockRegistry::getInstance();
        reg.addBlock(
            TexturePack::side3("./assets/image/block/GrassDirtBlock", ".png"));
        reg.addBlock(
            TexturePack::side1("./assets/image/block/DirtBlockSide.png"));
        reg.addBlock(
            TexturePack::side1("./assets/image/block/SandBlockSide.png"));
        reg.addBlock(
            TexturePack::side1("./assets/image/block/StoneBlockSide.png"));
        reg.addBlock(
            TexturePack::side1("./assets/image/block/WoodBlockSide.png"));
        reg.addBlock(
            TexturePack::side1("./assets/image/block/LeafBlockSide.png"));
        reg.addBlock(
            TexturePack::side1("./assets/image/block/WaterBlockTop.png"));
        sceneManager.put("play", std::make_shared<PlayScene>());
        sceneManager.put("test", std::make_shared<TestScene>());
        sceneManager.put("title", std::make_shared<TitleScene>());
        sceneManager.put("tutorial", std::make_shared<TutorialScene>());
        sceneManager.bind("title");
}

void MyGame::onUpdate() {
        auto win = gel::Game::getInstance()->getWindow();
        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
            (glfwGetKey(win, 'Q') == GLFW_PRESS &&
             glfwGetKey(win, 'X') == GLFW_PRESS)) {
                getContentManager()->unload();
                glfwSetWindowShouldClose(win, GL_TRUE);
                isExitNow = true;
                return;
        }
        sceneManager.update();
}

void MyGame::onDraw() {
        if (isExitNow) {
                return;
        }
        sceneManager.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
}

void MyGame::onFinish() { getContentManager()->unload(); }
