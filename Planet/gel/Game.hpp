#ifndef GEL_GAME_HPP
#define GEL_GAME_HPP
#include <mutex>
#include <thread>
#include "Thread.hpp"
#include "fbxsdk.hpp"
#include "gli.hpp"
namespace gel {
class ContentManager;
/**
 * Game is provide mainloop and many callback.
 * client will be extend this class for game develop.
 *
 * callback flow:
 * mainLoop
 * |
 * onInit
 * |
 * onLoad(on another thread)
 * |
 * *---onStart(on GL thread, when after a finished onLoad)
 * |
 * onUpdate
 * |
 * onDraw
 * |
 * other event(key, mouse, error, etc...)
 * |
 * onFinish
 * |
 * onExit
 */
class Game {
       public:
        explicit Game();
        virtual ~Game();
        /**
         * start a mainloop.
         * [CAUTION]
         * this method is NOT complete until finish game.
         * should be called at last of main function.
         * @param argc
         * @param argv
         * @param title
         * @param width
         * @param height
         * @param fullScreen
         * @return [STATUS-CODE]
         */
        int mainLoop(int argc, char* argv[], const char* title, int width,
                     int height, bool fullScreen);
        /**
         * return a game window.
         * @return
         */
        GLFWwindow* getWindow() const;

        /**
         * return a window width.
         * @return
         */
        int getWindowWidth() const;

        /**
         * return a window height;
         * @return
         */
        int getWindowHeight() const;

        /**
         * return a aspect
         * @return
         */
        float getWindowAspect() const;

        /**
         * return a window size.
         * @return
         */
        glm::vec2 getWindowSize() const;

        /**
         * @return
         */
        int getSolutionWidth() const;

        /**
         * @return
         */
        int getSolutionHeight() const;

        /**
         * @return
         */
        glm::vec2 getSolutionSize() const;

        /**
         * return a delta time.
         * @return
         */
        float getDeltaTime() const;

        /**
         * return a fbx manager.
         * @return
         */
        FbxManager* getFbxManager() const;

        /**
         * return a content manager.
         * @return
         */
        std::shared_ptr<ContentManager> getContentManager() const;

        /**
         * return a unique instance.
         * @return
         */
        static Game* getInstance();

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

       protected:
        static Game* instance;
        GLFWwindow* mWindow;
        FbxManager* fbxManager;
        int mWidth;
        int mHeight;
        int solutionWidth;
        int solutionHeight;
        double oldTime;
        float deltaTime;
        bool outputDebugMessage;
        std::thread loadThread;
        std::shared_ptr<ContentManager> contentManager;
        static void bridgeMouseButton(GLFWwindow* window, int button,
                                      int action, int mods);
        static void bridgeCursorMove(GLFWwindow* window, double x, double y);
        static void bridgeCursorEnter(GLFWwindow* window, int entered);
        static void bridgeScroll(GLFWwindow* window, double xOffset,
                                 double yOffset);
        static void bridgeKey(GLFWwindow* window, int key, int scan, int action,
                              int mods);
        static void bridgeChar(GLFWwindow* window, unsigned int codePoint);
        static void bridgeError(int error, const char* description);
        static void bridgeResize(GLFWwindow* window, int width, int height);
        static void APIENTRY bridgeDebugMessage(GLenum source, GLenum type,
                                                GLuint eid, GLenum severity,
                                                GLsizei length,
                                                const GLchar* message,
                                                GLvoid* user_param);
        static void bridgeExit();

        virtual void onMouseButton(GLFWwindow* window, int button, int action,
                                   int mods);
        virtual void onCursorMove(GLFWwindow* window, double x, double y);
        virtual void onCursorEnter(GLFWwindow* window, int entered);
        virtual void onScroll(GLFWwindow* window, double xOffset,
                              double yOffset);
        virtual void onKey(GLFWwindow* window, int key, int scan, int action,
                           int mods);
        virtual void onChar(GLFWwindow* window, unsigned int codePoint);
        virtual void onError(int error, const char* description);
        virtual void onResize(GLFWwindow* window, int width, int height);
        virtual void onDebugMessage(GLenum source, GLenum type, GLuint eid,
                                    GLenum severity, GLsizei length,
                                    const GLchar* message,
                                    const void* user_param);
        virtual void onExit();

        virtual void onInit();
        virtual void onLoad();
        virtual void onStart();
        virtual void onUpdate();
        virtual void onDraw();
        virtual void onFinish();

       private:
        bool finishedThread;
        std::mutex finishedMutex;
        int onGLInit(int argc, char* argv[], const char* title, int width,
                     int height, bool fullScreen);
        void onMainLoop1();
        void onMainLoop2();
        void onBackground();
};
}  // namespace gel
#endif