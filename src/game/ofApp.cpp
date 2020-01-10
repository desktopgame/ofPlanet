#include "ofApp.h"

#include <ctime>

#include "../io/File.hpp"
#include "../shader/Camera.hpp"
#include "../shader/CameraRegistry.hpp"
#include "../shader/Material.hpp"
#include "../shader/MaterialRegistry.hpp"
#include "../shader/ShaderRegistry.hpp"
#include "../world/Block.hpp"
#include "../world/TexturePack.hpp"
#include "../world/BlockPack.hpp"
#include "../world/TextureInfoCollection.hpp"
#include "../world/BlockInfoCollection.hpp"
#include "../objb/ObjBuilder.hpp"
#include "PlayScene.hpp"
namespace planet {

ofApp* ofApp::instance = nullptr;

ofApp::ofApp()
    : sceneManager(),
      inputSystem() {
        ofApp::instance = this;
}

//--------------------------------------------------------------
void ofApp::setup() {
        ofSeedRandom(time(NULL));
		ofBackground(ofColor::black);
		glm::vec3 size(1, 1, 1);
		using namespace objb;
		ObjBuilder builder;
		//0, 1, 2, 2, 3, 0
		builder.newModel("top")
			.vertex(glm::vec3(-size.x, 0, size.z))
			.vertex(glm::vec3(size.x, 0, size.z))
			.vertex(glm::vec3(-size.x, 0, -size.z))
			.vertex(glm::vec3(size.x, 0, -size.z))
			.normal(glm::vec3(0, 1, 0))
			.texcoord(glm::vec2(0, 0))
			.texcoord(glm::vec2(1, 0))
			.texcoord(glm::vec2(1, 1))
			.texcoord(glm::vec2(0, 1))
			.face(ObjFace{
				ObjPolygon(ObjIndex(1), ObjIndex(1), ObjIndex(1)),
				ObjPolygon(ObjIndex(2), ObjIndex(2), ObjIndex(1)),
				ObjPolygon(ObjIndex(4), ObjIndex(3), ObjIndex(1)),
				ObjPolygon(ObjIndex(3), ObjIndex(4), ObjIndex(1)),
			});
		builder.newModel("front")
			.vertex(glm::vec3(size.x, 0, -size.z))
			.vertex(glm::vec3(size.x, -2, -size.z))
			.vertex(glm::vec3(-size.x, 0, -size.z))
			.vertex(glm::vec3(-size.x, -2, -size.z))
			.normal(glm::vec3(0, 1, 0))
			.texcoord(glm::vec2(0, 0))
			.texcoord(glm::vec2(1, 0))
			.texcoord(glm::vec2(1, 1))
			.texcoord(glm::vec2(0, -1))
			.face(ObjFace{
				ObjPolygon(ObjIndex(1), ObjIndex(1), ObjIndex(1)),
				ObjPolygon(ObjIndex(2), ObjIndex(2), ObjIndex(1)),
				ObjPolygon(ObjIndex(4), ObjIndex(3), ObjIndex(1)),
				ObjPolygon(ObjIndex(3), ObjIndex(4), ObjIndex(1)),
			});
		builder.newModel("back")
			.vertex(glm::vec3(-size.x, 0, size.z))
			.vertex(glm::vec3(-size.x, -2, size.z))
			.vertex(glm::vec3(size.x, 0, size.z))
			.vertex(glm::vec3(size.x, -2, size.z))
			.normal(glm::vec3(0, 0, 1))
			.texcoord(glm::vec2(0, 0))
			.texcoord(glm::vec2(1, 0))
			.texcoord(glm::vec2(1, 1))
			.texcoord(glm::vec2(0, -1))
			.face(ObjFace{
				ObjPolygon(ObjIndex(1), ObjIndex(1), ObjIndex(1)),
				ObjPolygon(ObjIndex(2), ObjIndex(2), ObjIndex(1)),
				ObjPolygon(ObjIndex(4), ObjIndex(3), ObjIndex(1)),
				ObjPolygon(ObjIndex(3), ObjIndex(4), ObjIndex(1)),
			});
		builder.newModel("bottom")
			.vertex(glm::vec3(size.x, -2, size.z))
			.vertex(glm::vec3(-size.x, -2, size.z))
			.vertex(glm::vec3(size.x, -2, -size.z))
			.vertex(glm::vec3(-size.x, -2, -size.z))
			.normal(glm::vec3(0, 1, 0))
			.texcoord(glm::vec2(0, 0))
			.texcoord(glm::vec2(1, 0))
			.texcoord(glm::vec2(1, 1))
			.texcoord(glm::vec2(0, -1))
			.face(ObjFace{
				ObjPolygon(ObjIndex(1), ObjIndex(1), ObjIndex(1)),
				ObjPolygon(ObjIndex(2), ObjIndex(2), ObjIndex(1)),
				ObjPolygon(ObjIndex(4), ObjIndex(3), ObjIndex(1)),
				ObjPolygon(ObjIndex(3), ObjIndex(4), ObjIndex(1)),
			});
		builder.newModel("right")
			.vertex(glm::vec3(size.x, 0, size.z))
			.vertex(glm::vec3(size.x, -2, size.z))
			.vertex(glm::vec3(size.x, 0, -size.z))
			.vertex(glm::vec3(size.x, -2, -size.z))
			.normal(glm::vec3(0, 1, 0))
			.texcoord(glm::vec2(0, 0))
			.texcoord(glm::vec2(1, 0))
			.texcoord(glm::vec2(1, 1))
			.texcoord(glm::vec2(0, -1))
			.face(ObjFace{
				ObjPolygon(ObjIndex(1), ObjIndex(1), ObjIndex(1)),
				ObjPolygon(ObjIndex(2), ObjIndex(2), ObjIndex(1)),
				ObjPolygon(ObjIndex(4), ObjIndex(3), ObjIndex(1)),
				ObjPolygon(ObjIndex(3), ObjIndex(4), ObjIndex(1)),
			});
		builder.newModel("left")
			.vertex(glm::vec3(-size.x, 0, -size.z))
			.vertex(glm::vec3(-size.x, -2, -size.z))
			.vertex(glm::vec3(-size.x, 0, size.z))
			.vertex(glm::vec3(-size.x, -2, size.z))
			.normal(glm::vec3(-1, 0, 0))
			.texcoord(glm::vec2(0, 0))
			.texcoord(glm::vec2(1, 0))
			.texcoord(glm::vec2(1, 1))
			.texcoord(glm::vec2(0, -1))
			.face(ObjFace{
				ObjPolygon(ObjIndex(1), ObjIndex(1), ObjIndex(1)),
				ObjPolygon(ObjIndex(2), ObjIndex(2), ObjIndex(1)),
				ObjPolygon(ObjIndex(4), ObjIndex(3), ObjIndex(1)),
				ObjPolygon(ObjIndex(3), ObjIndex(4), ObjIndex(1)),
			});
		File::writeAllText("C:\\Work\\myobj.obj", builder.toString());
#if _DEBUG
        glDebugMessageCallback(
            reinterpret_cast<GLDEBUGPROC>(ofApp::bridgeDebugMessage), NULL);
        glEnable(GL_DEBUG_OUTPUT);
#endif
        CameraRegistry::setDefaultScreenSize(glm::vec2(1280, 720), false);
        auto cam = CameraRegistry::create("Block");
        auto mat = MaterialRegistry::create("Block");
        CameraRegistry::create("BlockInv");
        mat->ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        mat->diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        mat->specular = glm::vec4(0.f, 0.f, 0.f, 1.0f);
        mat->shininess = 50;
        cam->rehash();
        ShaderRegistry::loadFile("Block", "shaders/block.vert",
                                 "shaders/block.frag");
        ShaderRegistry::loadFile("Skybox", "shaders/skybox.vert",
                                 "shaders/skybox.frag");
        ShaderRegistry::loadFile("Sprite", "shaders/sprite.vert",
                                 "shaders/sprite.frag");
        ShaderRegistry::loadFile("Model", "shaders/model.vert",
                                 "shaders/model.frag");
        ShaderRegistry::loadFile("FBX", "shaders/fbx.vert", "shaders/fbx.frag");
		ShaderRegistry::loadFile("AABB", "shaders/aabb.vert", "shaders/aabb.frag");

		TextureInfoCollection tic;
		tic.deserialize(File::readAllText("./textures.json"));
		BlockInfoCollection bic;
		bic.deserialize(File::readAllText("./blocks.json"));
		BlockPack::load(bic);
        TexturePack::load(tic);
		TexturePack::getCurrent()->resolve();
        sceneManager.put("Play", std::make_shared<PlayScene>());
        sceneManager.bind("Play");
}

//--------------------------------------------------------------
void ofApp::update() {
        inputSystem.updateBuffers();
        sceneManager.update();
}

//--------------------------------------------------------------
void ofApp::draw() { sceneManager.draw(); }

void ofApp::exit(ofEventArgs& args) {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

std::shared_ptr<KeyboardState> ofApp::getKeyboardState() {
        return ofApp::instance->inputSystem.getKeyboardState();
}

std::shared_ptr<MouseState> ofApp::getMouseState() {
        return ofApp::instance->inputSystem.getMouseState();
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
// protected
void ofApp::bridgeDebugMessage(GLenum source, GLenum type, GLuint eid,
                               GLenum severity, GLsizei length,
                               const GLchar* message, GLvoid* user_param) {
        // https://gist.github.com/liam-middlebrook/c52b069e4be2d87a6d2f
        const char* _source;
        const char* _type;
        const char* _severity;
        if (eid == 131218) {
                return;
        }
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
        bool outputDebugMessage = true;
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
}
}  // namespace planet