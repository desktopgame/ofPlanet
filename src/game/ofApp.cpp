#include "ofApp.h"

#include <ofImage.h>
#include <soil.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <ctime>
#include <sstream>
#include <string>
#include <future>

#include "../common/GLM.hpp"
#include "../common/glfw.hpp"
#include "../io/Directory.hpp"
#include "../io/File.hpp"
#include "../io/Path.hpp"
#include "../objb/MtlBuilder.hpp"
#include "../objb/ObjBuilder.hpp"
#include "../shader/Camera.hpp"
#include "../shader/CameraRegistry.hpp"
#include "../shader/Material.hpp"
#include "../shader/MaterialRegistry.hpp"
#include "../shader/ShaderRegistry.hpp"
#include "../shader/Texture.hpp"
#include "../text/Strings.hpp"
#include "../world/Block.hpp"
#include "../world/BlockPack.hpp"
#include "../world/TexturePack.hpp"
#include "../world/WorldIO.hpp"
#include "../world/biome/HillBiome.hpp"
#include "../world/biome/PlainBiome.hpp"
#include "../world/biome/ScriptBiome.hpp"
namespace planet {

using ExportMode =
    enum { EXPORT_JSON = 0,
           EXPORT_OBJ,
           EXPORT_BMP,
    };

ofApp::ofApp()
    : planet(std::make_shared<Planet>(createPlaneNameSet())),
      rand(),
      cameraAngle(0),
      gui(),
      playMode(false),
      fpsCon(),
      biomeNames("Biome"),
      exportTypes(),
      exportDir("Output Directory"),
      cameraSpeed("CameraSpeed", 0.01f),
      worldSize("Size", 2),
	splitCount("SplitCount", 0),
      asyncOp(nullptr) {
        exportDir.setString("dist");
        worldSize.value = glm::vec3(128, 64, 128);
        cameraSpeed.value = 0.01f;
        fpsCon.enable();
        fpsCon.setRotateSpeed(1.0f);
        fpsCon.setMoveSpeed(1.0f);
        fpsCon.setMode(FirstPersonController::Mode::Key);
        gui.setup();
        auto files = Directory::files("./data/script", false);
        for (int i = 0; i < static_cast<int>(files.size()); i++) {
                auto file = files.at(i);
                biomes.emplace_back(
                    std::make_shared<planet::ScriptBiome>(file));
                biomeNames.items.emplace_back(file);
        }
        biomeNames.rehash();
        exportTypes.labels = std::vector<std::string>{"JSON", "OBJ", "BMP"};
		splitCount.step = 2;
		splitCount.min = 0;
		splitCount.max = 8;
}

//--------------------------------------------------------------
void ofApp::setup() {
        ofSeedRandom(time(NULL));
        ofBackground(ofColor::black);
#if _DEBUG
        glDebugMessageCallback(
            reinterpret_cast<GLDEBUGPROC>(ofApp::bridgeDebugMessage), NULL);
        glEnable(GL_DEBUG_OUTPUT);
#endif
        // カメラ, マテリアルの設定
        CameraRegistry::setDefaultScreenSize(glm::vec2(1280, 720), false);
        auto cam = CameraRegistry::create("Block");
        auto mat = MaterialRegistry::create("Block");
        mat->ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        mat->diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        mat->specular = glm::vec4(0.f, 0.f, 0.f, 1.0f);
        mat->shininess = 50;
        cam->rehash();
        // シェーダの読み込み
        ShaderRegistry::loadFile("Block", "shaders/block.vert",
                                 "shaders/block.frag");
        // 設定ファイルの読み込み
        TextureInfoCollection tic;
        tic.deserialize(File::readAllText("./textures.json"));
        BlockInfoCollection bic;
        bic.deserialize(File::readAllText("./blocks.json"));
        BlockPack::load(bic);
        TexturePack::load(tic);
        TexturePack::getCurrent()->resolve();
        // 描画設定
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//--------------------------------------------------------------
void ofApp::update() {
        auto w = planet->getWorld();
        if (!w) {
                return;
        }
        w->update();
        if (this->playMode.get()) {
			cameraUser();
        } else {
			cameraAuto();
        }
}

//--------------------------------------------------------------
void ofApp::draw() {
        // 3D機能を有効にして画面を描画
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        auto w = planet->getWorld();
        planet->drawToBuffer();
        planet->render();
        // 3D機能を無効にして設定画面を描画
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        gui.begin();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
        // --SettingWindowの表示
        ImGui::Begin("Setting");
        worldSize.draw();
        biomeNames.draw();
        cameraSpeed.draw();
        if (ImGui::Button("Generate") && biomeNames.selectedIndex >= 0 &&
            !biomes.empty()) {
                planet->generate(worldSize.value,
                                 biomes.at(biomeNames.selectedIndex));
        }
        ImGui::Checkbox("PlayMode", &playMode.getNewValue());
        playMode.detect();
        ImGui::End();
        // --ParameterWindowの表示
        ImGui::Begin("Parameter");
        biomes.at(biomeNames.selectedIndex)->onGUI();
        ImGui::End();
        // --ExporterWindowの表示
        ImGui::Begin("Exporter");
        exportTypes.draw();
		splitCount.draw();
        int exportMode = exportTypes.mode;
        exportDir.draw();
        // 処理中ならラベルだけを表示
        bool processing = isProcessing();
        if (processing) {
                char buf[256];
                std::memset(buf, '\0', 256);
                std::sprintf(buf, "processing now... %f %%",
                             (this->asyncOp->getValue() * 100.0f));
                ImGui::Text(buf);
        } else {
                //そうでなければボタンを表示
                if (ImGui::Button("Export")) {
                        Directory::create(exportDir.getString());
                        if (exportMode == EXPORT_JSON) {
                                exportJson(Path::build(std::vector<std::string>{
                                    exportDir.getString(), "data.json"}));
                        } else if (exportMode == EXPORT_OBJ) {
                                exportObj(exportDir.getString());
                        } else if (exportMode == EXPORT_BMP) {
                                exportBmp(Path::build(std::vector<std::string>{
                                    exportDir.getString(), "data.bmp"}));
                        }
                }
        }
        ImGui::End();
        ImGui::PopStyleColor();
        gui.end();
}

void ofApp::exit(ofEventArgs& args) {}

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
void ofApp::windowResized(int w, int h) {
        auto cam = CameraRegistry::ref("Block");
        cam->setScreenSize(glm::vec2(w, h));
        cam->rehash();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
// protected
void ofApp::cameraAuto() {
	auto w = planet->getWorld();
	const int wsx = w->getXSize();
	const int wsy = w->getYSize();
	const int wsz = w->getZSize();
	const float fwsx = static_cast<float>(w->getXSize());
	const float fwsy = static_cast<float>(w->getYSize());
	const float fwsz = static_cast<float>(w->getZSize());
	const float hfwsx = fwsx / 2;
	const float hfwsz = fwsz / 2;
	auto myCam = CameraRegistry::get("Block");
	// プレイモードではないので、オブジェクトの周りを周回します。
	auto cx = std::cos(cameraAngle);
	auto cz = std::sin(cameraAngle);
	myCam->setPosition(
		glm::vec3(hfwsx + (hfwsx * cx), wsy, hfwsz + (hfwsz * cz)));
	myCam->setLookAt(glm::vec3(wsx / 2, 0, wsz / 2));
	myCam->rehash();
	this->cameraAngle += cameraSpeed.value;
}
void ofApp::cameraUser() {
	auto w = planet->getWorld();
	const int wsx = w->getXSize();
	const int wsy = w->getYSize();
	const int wsz = w->getZSize();
	const int OF_KEY_SPACE = 32;
	auto myCam = CameraRegistry::get("Block");
	// WASD, 矢印キーによる移動と回転
	fpsCon.update();
	if (playMode.testIsChanged()) {
		myCam->setPosition(
			glm::vec3(wsx / 2, wsy / 2, wsz / 2));
	}
	else {
		myCam->setPosition(myCam->getPosition() +
			fpsCon.getVelocity());
	}
	myCam->setLookAt(myCam->getPosition() +
		fpsCon.getTransform().forward());
	// 上昇, 下降
	if (ofGetKeyPressed(OF_KEY_SPACE)) {
		myCam->setPosition(myCam->getPosition() +
			glm::vec3(0, 0.4f, 0));
	}
	else if (glfw::getKey(glfw::Key_left_shift) ||
		glfw::getKey(glfw::Key_z)) {
		myCam->setPosition(myCam->getPosition() +
			glm::vec3(0, -0.4f, 0));
	}
	myCam->rehash();
}
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

void ofApp::exportJson(const std::string& outputFile) {
        if (!isProcessing()) {
                File::remove(outputFile);
                this->asyncOp = WorldIO::toJson(outputFile, planet->getWorld());
        }
}

void ofApp::exportObj(const std::string& outputDir) {
		if (isProcessing()) {
			return;
		}
		if (splitCount.value <= 1) {
			auto outputFile = Path::build(
				std::vector<std::string>{outputDir, "data.obj"});
			File::remove(outputFile);
			File::remove(outputFile + ".mtl");
			this->asyncOp = WorldIO::toObj(outputDir, planet->getWorld());
		} else {
			char buf[64];
			std::memset(buf, '\0', 64);
			std::string cpOutputDir = outputDir;
			auto worlds = planet->getWorld()->split(splitCount.value);
			int splitCountN = static_cast<int>(worlds.size());
			auto asyncs = std::vector<AsyncOperation>();
			for (int i = 0; i < splitCountN; i++) {
				auto wpart = worlds.at(i);
				std::sprintf(buf, "_Split_x%dz%d", wpart.offset.x, wpart.offset.z);

				auto newOutputDir = cpOutputDir + std::string(buf);
				Directory::create(newOutputDir);
				auto outputFile = Path::build(
					std::vector<std::string>{newOutputDir, "data.obj"});
				File::remove(outputFile);
				File::remove(outputFile + ".mtl");
				asyncs.emplace_back(WorldIO::toObj(newOutputDir, wpart.world));
			}
			auto aAsync = std::make_shared<Progress>();
			this->asyncOp = aAsync;
			std::thread([aAsync, asyncs, splitCountN]() -> void {
				bool run = true;
				int count = 0;
				while (run) {
					run = false;
					count = 0;
					// すべて終わるまで待機
					for (auto aa : asyncs) {
						if (!aa->isDone()) {
							run = true;
						} else {
							count++;
						}
					}
					aAsync->setValue(static_cast<float>(count) / static_cast<float>(splitCountN));
				}
				aAsync->setValue(1.0f);
			}).detach();
		}
}

void ofApp::exportBmp(const std::string& outputFile) {
        if (!isProcessing()) {
                File::remove(outputFile);
                this->asyncOp = WorldIO::toBmp(outputFile, planet);
        }
}

bool ofApp::isProcessing() const {
        return this->asyncOp != nullptr && !this->asyncOp->isDone();
}

NameSet ofApp::createPlaneNameSet() {
        NameSet set;
        set.shader = "Block";
        set.camera = "Block";
        set.material = "Block";

        set.aVertex.enable();
        set.aNormal.enable();
        set.aUV.enable();

        set.uAmbient.enable();
        set.uDiffuse.enable();
        set.uSpecular.enable();
        set.uTexture.enable();
        set.uShininess.enable();
        set.aPosition.enable();

        set.uMVPMatrix.enable();
        set.uNormalMatrix.enable();
        return set;
}

NameSet ofApp::createGunNameSet() {
        NameSet nameSet;
        nameSet.shader = "ModelTexture";
        nameSet.camera = "Block";
        nameSet.material = "Block";
        return nameSet;
}
}  // namespace planet