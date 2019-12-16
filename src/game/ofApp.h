#pragma once

#include <ofMain.h>

#include "../input/InputSystem.hpp"
#include "../input/KeyboardState.hpp"
#include "../input/MouseState.hpp"
#include "../scene/SceneManager.hpp"
namespace planet {

class ofApp : public ofBaseApp {
       public:
        ofApp();
        void setup();
        void update();
        void draw();
        void exit(ofEventArgs& args);

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        static std::shared_ptr<KeyboardState> getKeyboardState();
        static std::shared_ptr<MouseState> getMouseState();

        static const glm::ivec3 worldSize;

       protected:
        static ofApp* instance;
        static void bridgeDebugMessage(GLenum source, GLenum type, GLuint eid,
                                       GLenum severity, GLsizei length,
                                       const GLchar* message,
                                       GLvoid* user_param);

       private:
        SceneManager sceneManager;
        InputSystem inputSystem;
};
}  // namespace planet