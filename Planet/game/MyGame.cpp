#include "MyGame.hpp"
#include "scene/LoadScene.hpp"
#include "scene/PlayScene.hpp"
#include "scene/TestScene.hpp"
#include "scene/TitleScene.hpp"
#include "scene/TutorialScene.hpp"
MyGame::MyGame()
    : sceneManager(),
      gameDevice(gel::GameDevice::make_shared("./assets")),
      isExitNow(false) {}

void MyGame::onInit() {
        this->outputDebugMessage = false;
        // init shader
        gel::ShaderRegistry& sreg = gel::ShaderRegistry::getInstance();
        sreg.put("Texture2D", "./texture2D.vert", "./texture2D.frag");
        sreg.put("Texture3D", "./texture.vert", "./texture.frag");
        sreg.put("TextureFixed", "./textureFixed.vert", "./textureFixed.frag");
        sreg.put("ColorFixed", "./colorFixed.vert", "./colorFixed.frag");
        sreg.put("Screen", "./screen.vert", "./screen.frag");
        sreg.put("SkyBox", "./skybox.vert", "./skybox.frag");
        sreg.put("Color", "./color.vert", "./color.frag");
        sreg.put("Noise", "./noise.vert", "./noise.frag");
        sreg.put("CRT", "./crt.vert", "./crt.frag");
        // Anisotropic filtering
        float filter;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &filter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, filter);
        sceneManager.put("load", std::make_shared<LoadScene>(gameDevice));
        sceneManager.bind("load");
        gameDevice->getContentManager()->onContentLoad().connect(
            [](gel::ContentLoadEvent e) { std::cout << e.path << std::endl; });
}

void MyGame::onLoad() {
        gameDevice->getContentManager()->load(Thread::OnBackground);
}

void MyGame::onStart() {
        gameDevice->getContentManager()->load(Thread::OnGL);
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
        sceneManager.put("play", std::make_shared<PlayScene>(gameDevice));
        sceneManager.put("test", std::make_shared<TestScene>(gameDevice));
        sceneManager.put("title", std::make_shared<TitleScene>(gameDevice));
        sceneManager.put("tutorial",
                         std::make_shared<TutorialScene>(gameDevice));
        sceneManager.bind("play");
}

void MyGame::onUpdate() {
        auto win = gel::Game::getInstance()->getWindow();
        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
            (glfwGetKey(win, 'Q') == GLFW_PRESS &&
             glfwGetKey(win, 'X') == GLFW_PRESS)) {
                gameDevice->getContentManager()->unload();
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
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
}

void MyGame::onFinish() { gameDevice->getContentManager()->unload(); }
