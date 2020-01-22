#pragma once

#include <ofEasyCam.h>
#include <ofMain.h>
#include <ofShader.h>
#include <ofxImGui.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "../common/FirstPersonController.hpp"
#include "../common/Random.hpp"
#include "../common/StateValue.hpp"
#include "../imguiex/CheckBox.hpp"
#include "../imguiex/Integer.hpp"
#include "../imguiex/Float.hpp"
#include "../imguiex/FloatXZ.hpp"
#include "../imguiex/InputField.hpp"
#include "../imguiex/ListBox.hpp"
#include "../imguiex/RadioButtonGroup.hpp"
#include "../shader/Plane.hpp"
#include "../shader/Transform.hpp"
#include "../world/Planet.hpp"
#include "../world/World.hpp"
#include "../world/WorldIO.hpp"
#include "../world/biome/Biome.hpp"

namespace objb {
class ObjBuilder;
class MtlBuilder;
}  // namespace objb

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

       protected:
		void cameraAuto();
		void cameraUser();
        static void bridgeDebugMessage(GLenum source, GLenum type, GLuint eid,
                                       GLenum severity, GLsizei length,
                                       const GLchar* message,
                                       GLvoid* user_param);

        bool isProcessing() const;
        void exportJson(const std::string& outputFile);
        void exportObj(const std::string& outputDir);
        void exportBmp(const std::string& outputFile);

        ofEasyCam cam;
        std::shared_ptr<Planet> planet;
        Random rand;
        float cameraAngle;

        ofxImGui::Gui gui;
        StateValue<bool> playMode;
        FirstPersonController fpsCon;
        std::vector<std::shared_ptr<Biome> > biomes;
        imguiex::ListBox biomeNames;
        imguiex::RadioButtonGroup exportTypes;
        imguiex::InputField<> exportDir;
        imguiex::Float cameraSpeed;
        imguiex::FloatXZ worldSize;
		imguiex::Integer splitCount;
        AsyncOperation asyncOp;

        static NameSet createPlaneNameSet();
        static NameSet createGunNameSet();
};
}  // namespace planet