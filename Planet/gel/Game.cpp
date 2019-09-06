#include "Game.hpp"
#include <AL/alut.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include "gel.hpp"
#include "ui/GUI.hpp"
namespace gel {
Game* Game::instance = nullptr;

Game::Game()
    : model(1.0f),
      view(1.0f),
      projection(1.0f),
      outputDebugMessage(true),
      deltaTime(0) {
        if (Game::instance != nullptr) {
                throw std::logic_error("should be game instance single");
        }
        Game::instance = this;
}

Game::~Game() {}

int Game::mainLoop(int argc, char* argv[], const char* title, int width,
                   int height, bool fullScreen) {
        std::atexit(bridgeExit);
        this->mWidth = width;
        this->mHeight = height;
        this->solutionWidth = width;
        this->solutionHeight = height;
        // glutInit(&argc, argv);
        if (!glfwInit()) return -1;
        if (!alutInit(&argc, argv)) return -1;
        // create window
        GLFWwindow* window = NULL;
        if (fullScreen) {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
                window = glfwCreateWindow(mode->width, mode->height, title,
                                          monitor, NULL);
                this->mWidth = width = mode->width;
                this->mHeight = height = mode->height;
        } else {
                glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
                window = glfwCreateWindow(width, height, title, NULL, NULL);
        }
        this->mWindow = window;
        if (!window) {
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window);
        if (glewInit() != GLEW_OK) return -1;
        // glslInit
        glfwSetMouseButtonCallback(window, Game::bridgeMouseButton);
        glfwSetCursorPosCallback(window, Game::bridgeCursorMove);
        glfwSetCursorEnterCallback(window, Game::bridgeCursorEnter);
        glfwSetScrollCallback(window, Game::bridgeScroll);
        glfwSetKeyCallback(window, Game::bridgeKey);
        glfwSetCharCallback(window, Game::bridgeChar);
        glfwSetWindowSizeCallback(window, Game::bridgeResize);
#if DEBUG
        glfwSetErrorCallback(Game::bridgeError);
        glDebugMessageCallback(
            reinterpret_cast<GLDEBUGPROC>(Game::bridgeDebugMessage), NULL);
        glEnable(GL_DEBUG_OUTPUT);
        std::cout << "OpenGL" << std::endl;
        std::cout << "    Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "    GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
                  << std::endl;
#endif
        // init fbx manager
        this->fbxManager = FbxManager::Create();
        if (this->fbxManager == NULL) {
                ::fprintf(stderr, "fatal error: FbxManager::Create()");
                ::abort();
        }
        init();
        glfwSetTime(0.0);
        glfwSwapInterval(1);
        // init Imgui
#if DEBUG
        gui::internal::init(window);
#endif
        // start mainloop
        while (!glfwWindowShouldClose(window)) {
#if DEBUG
                gui::internal::newFrame();
#endif
                this->oldTime = glfwGetTime();
                update();
                draw();
                double nowTime = glfwGetTime();
                this->deltaTime = nowTime - oldTime;
                this->oldTime = nowTime;

#if DEBUG
                gui::internal::endFrame();
#endif
        }
#if DEBUG
        gui::internal::destroy();
#endif
        fbxManager->Destroy();
        alutExit();
        glfwTerminate();
        glfwDestroyWindow(mWindow);
        return 0;
}

GLFWwindow* Game::getWindow() const { return this->mWindow; }

float Game::getDeltaTime() const { return this->deltaTime; }

FbxManager* Game::getFbxManager() const { return fbxManager; }

Game* Game::getInstance() { return instance; }

int Game::getWindowWidth() const { return mWidth; }

int Game::getWindowHeight() const { return mHeight; }

float Game::getWindowAspect() const { return ((float)mWidth / (float)mHeight); }

glm::vec2 Game::getWindowSize() const { return glm::vec2(mWidth, mHeight); }

int Game::getSolutionWidth() const { return solutionWidth; }

int Game::getSolutionHeight() const { return solutionHeight; }

glm::vec2 Game::getSolutionSize() const {
        return glm::vec2(solutionWidth, solutionHeight);
}

// protecteds
void Game::init() {}
void Game::update() {}

void Game::draw() {}

void Game::bridgeMouseButton(GLFWwindow* window, int button, int action,
                             int mods) {
        Game::instance->onMouseButton(window, button, action, mods);
}

void Game::bridgeCursorMove(GLFWwindow* window, double x, double y) {
        Game::instance->onCursorMove(window, x, y);
}

void Game::bridgeCursorEnter(GLFWwindow* window, int entered) {
        Game::instance->onCursorEnter(window, entered);
}

void Game::bridgeScroll(GLFWwindow* window, double xOffset, double yOffset) {
        Game::instance->onScroll(window, xOffset, yOffset);
}

void Game::bridgeKey(GLFWwindow* window, int key, int scan, int action,
                     int mods) {
        Game::instance->onKey(window, key, scan, action, mods);
}

void Game::bridgeChar(GLFWwindow* window, unsigned int codePoint) {
        Game::instance->onChar(window, codePoint);
}

void Game::bridgeError(int error, const char* description) {
        Game::instance->onError(error, description);
}

void Game::bridgeResize(GLFWwindow* window, int width, int height) {
        Game::instance->onResize(window, width, height);
}

void Game::bridgeDebugMessage(GLenum source, GLenum type, GLuint eid,
                              GLenum severity, GLsizei length,
                              const GLchar* message, GLvoid* user_param) {
        instance->onDebugMessage(source, type, eid, severity, length, message,
                                 user_param);
}

void Game::bridgeExit() { instance->onExit(); }

void Game::onMouseButton(GLFWwindow* window, int button, int action, int mods) {
}

void Game::onCursorMove(GLFWwindow* window, double x, double y) {}

void Game::onCursorEnter(GLFWwindow* window, int entered) {}

void Game::onScroll(GLFWwindow* window, double xOffset, double yOffset) {}

void Game::onKey(GLFWwindow* window, int key, int scan, int action, int mods) {}

void Game::onChar(GLFWwindow* window, unsigned int codePoint) {}

void Game::onError(int error, const char* description) {
        std::cerr << description << std::endl;
}

void Game::onResize(GLFWwindow* window, int width, int height) {
        this->mWidth = width;
        this->mHeight = height;
}

void Game::onDebugMessage(GLenum source, GLenum type, GLuint eid,
                          GLenum severity, GLsizei length,
                          const GLchar* message, const void* user_param) {
#if DEBUG
        // https://gist.github.com/liam-middlebrook/c52b069e4be2d87a6d2f
        const char* _source;
        const char* _type;
        const char* _severity;

        switch (source) {
                case GL_DEBUG_SOURCE_API:
                        _source = "API";
                        break;

                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                        _source = "WINDOW SYSTEM";
                        break;

                case GL_DEBUG_SOURCE_SHADER_COMPILER:
                        _source = "SHADER COMPILER";
                        break;

                case GL_DEBUG_SOURCE_THIRD_PARTY:
                        _source = "THIRD PARTY";
                        break;

                case GL_DEBUG_SOURCE_APPLICATION:
                        _source = "APPLICATION";
                        break;

                case GL_DEBUG_SOURCE_OTHER:
                        _source = "UNKNOWN";
                        break;

                default:
                        _source = "UNKNOWN";
                        break;
        }

        switch (type) {
                case GL_DEBUG_TYPE_ERROR:
                        _type = "ERROR";
                        break;

                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                        _type = "DEPRECATED BEHAVIOR";
                        break;

                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                        _type = "UDEFINED BEHAVIOR";
                        break;

                case GL_DEBUG_TYPE_PORTABILITY:
                        _type = "PORTABILITY";
                        break;

                case GL_DEBUG_TYPE_PERFORMANCE:
                        _type = "PERFORMANCE";
                        break;

                case GL_DEBUG_TYPE_OTHER:
                        _type = "OTHER";
                        break;

                case GL_DEBUG_TYPE_MARKER:
                        _type = "MARKER";
                        break;

                default:
                        _type = "UNKNOWN";
                        break;
        }

        switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:
                        _severity = "HIGH";
                        break;

                case GL_DEBUG_SEVERITY_MEDIUM:
                        _severity = "MEDIUM";
                        break;

                case GL_DEBUG_SEVERITY_LOW:
                        _severity = "LOW";
                        break;

                case GL_DEBUG_SEVERITY_NOTIFICATION:
                        _severity = "NOTIFICATION";
                        break;

                default:
                        _severity = "UNKNOWN";
                        break;
        }
        if (outputDebugMessage) {
                std::stringstream ss;
                ss << "source(" << _source << ") ";
                ss << "type(" << _type << ") ";
                ss << "eid(" << eid << ") ";
                ss << "severity(" << _severity << ") ";
                ss << "length(" << length << ") ";
                ss << "message(" << message << ")";
                std::cout << ss.str() << std::endl;
        }
#endif
}
void Game::onExit() {}
}  // namespace  gel
