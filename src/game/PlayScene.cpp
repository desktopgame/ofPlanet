#include "PlayScene.hpp"

#include <ofImage.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <string>
#include <sstream>
#include <soil.h>

#include "../common/glfw.hpp"
#include "../common/GLM.hpp"
#include "../text/Strings.hpp"
#include "../io/Path.hpp"
#include "../io/File.hpp"
#include "../input/Input.hpp"
#include "../shader/Camera.hpp"
#include "../shader/CameraRegistry.hpp"
#include "../shader/ShaderRegistry.hpp"
#include "../shader/Texture.hpp"
#include "../world/Block.hpp"
#include "../world/biome/PlainBiome.hpp"
#include "../world/biome/HillBiome.hpp"
#include "../world/biome/PlainBiome.hpp"
#include "../world/biome/ScriptBiome.hpp"
#include "../world/WorldIO.hpp"
#include "../io/Directory.hpp"
#include "../objb/ObjBuilder.hpp"
#include "../objb/MtlBuilder.hpp"
#include "Screen.hpp"
namespace planet {
using ExportMode = enum {
	EXPORT_JSON = 0,
	EXPORT_OBJ,
	EXPORT_BMP,
};
PlayScene::PlayScene()
    : Scene(),
      planet(std::make_shared<Planet>(createPlaneNameSet())),
      rand(),
      cameraAngle(0),
      gui(),
      playMode(false),
      fpsCon(),
      biomeNames("Biome"),
      exportTypes(),
	exportFile("File"),
	cameraSpeed("CameraSpeed", 0.01f),
	worldSize("Size", 2) {
	exportFile.setString("./data");
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
		biomes.emplace_back(std::make_shared<planet::ScriptBiome>(file));
		biomeNames.items.emplace_back(file);
	}
	biomeNames.rehash();
	exportTypes.labels = std::vector<std::string>{ "JSON", "OBJ", "BMP" };
}

PlayScene::~PlayScene() {}

void PlayScene::show() {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// set title mode
		auto w = planet->getWorld();
		if (w) {
			auto cam = CameraRegistry::get("Block");
			cam->setPosition(glm::vec3(w->getXSize(), w->getYSize(), w->getZSize()));
			cam->setLookAt(glm::vec3(w->getXSize() / 2, 0, w->getZSize() / 2));
			cam->rehash();
			w->setPlayMode(false);
		}
}

void PlayScene::update() {
		playUpdate();
}

void PlayScene::draw() {
		playDraw();
}

void PlayScene::hide() {}

std::string PlayScene::getNextScene() const { return ""; }

bool PlayScene::isFinished() const { return false; }

void PlayScene::playUpdate() {
	auto w = planet->getWorld();
	if (!w) {
		return;
	}
	w->update();
	const int wsx = w->getXSize();
	const int wsy = w->getYSize();
	const int wsz = w->getZSize();
	const float fwsx = static_cast<float>(w->getXSize());
	const float fwsy = static_cast<float>(w->getYSize());
	const float fwsz = static_cast<float>(w->getZSize());
	const float hfwsx = fwsx / 2;
	const float hfwsz = fwsz / 2;
	// updfate camera
	auto myCam = CameraRegistry::get("Block");
	if (this->playMode.get()) {
		fpsCon.update();
		if (playMode.testIsChanged()) {
			myCam->setPosition(glm::vec3(wsx / 2, wsy / 2, wsz / 2));
		} else {
			myCam->setPosition(myCam->getPosition() + fpsCon.getVelocity());
		}
		myCam->setLookAt(myCam->getPosition() + fpsCon.getTransform().forward());
		// debug
		if (Input::getKey(glfw::Key_space)) {
			myCam->setPosition(myCam->getPosition() + glm::vec3(0, 0.4f, 0));
		} else if (Input::getKey(glfw::Key_left_shift) || Input::getKey(glfw::Key_z)) {
			myCam->setPosition(myCam->getPosition() + glm::vec3(0, -0.4f, 0));
		}
		myCam->rehash();
	} else {
		auto cx = std::cos(cameraAngle);
		auto cz = std::sin(cameraAngle);
		myCam->setPosition(glm::vec3(hfwsx + (hfwsx * cx), wsy, hfwsz + (hfwsz * cz)));
		myCam->setLookAt(glm::vec3(wsx / 2, 0, wsz / 2));
		myCam->rehash();
		this->cameraAngle += cameraSpeed.value;
	}
}

void PlayScene::playDraw() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	auto w = planet->getWorld();
	planet->drawToBuffer();
	planet->render();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	gui.begin();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
	// Setting Window
	ImGui::Begin("Setting");
	worldSize.draw();
	biomeNames.draw();
	//ImGui::ListBox("Biome", &biomeIndex, biomeList, IM_ARRAYSIZE(biomeList), 4);
	cameraSpeed.draw();
	if (ImGui::Button("Generate") && biomeNames.selectedIndex >= 0 && !biomes.empty()) {
		planet->generate(worldSize.value, biomes.at(biomeNames.selectedIndex));
	}
	ImGui::Checkbox("PlayMode", &playMode.getNewValue());
	playMode.detect();
	ImGui::End();
	// Parameter Window
	ImGui::Begin("Parameter");
	biomes.at(biomeNames.selectedIndex)->onGUI();
	ImGui::End();
	// Exporter Window
	ImGui::Begin("Exporter");
	//[JSON] [OBJ]
	exportTypes.draw();
	int exportMode = exportTypes.mode;
	exportFile.draw();
	if (ImGui::Button("Export")) {
		std::string outputFile = exportFile.getString();
		if (exportMode == EXPORT_JSON) {
			outputFile = Strings::fixsuffix(outputFile, ".json");
			exportJson(outputFile);
		} else if (exportMode == EXPORT_OBJ) {
			outputFile = Strings::fixsuffix(outputFile, ".obj");
			exportObj(outputFile);
		} else if (exportMode == EXPORT_BMP) {
			outputFile = Strings::fixsuffix(outputFile, ".bmp");
			exportBmp(outputFile);
		}
	}
	ImGui::End();
	ImGui::PopStyleColor();
	gui.end();
}

void PlayScene::exportJson(const std::string & outputFile) {
	File::remove(outputFile);
	WorldIO::toJson(outputFile, planet->getWorld());
}

void PlayScene::exportObj(const std::string & outputFile) {
	File::remove(outputFile);
	auto w = planet->getWorld();
	using namespace objb;
	ObjBuilder ob;
	MtlBuilder mb;
	int xsize = w->getXSize();
	int ysize = w->getYSize();
	int zsize = w->getZSize();
	glm::vec3 size(1, 1, 1);
	for (int x = 0; x < xsize; x++) {
		for (int y = 0; y < ysize; y++) {
			for (int z = 0; z < zsize; z++) {
				auto block = w->getBlockBehavior(x, y, z);
				if (!block) {
					continue;
				}
				bool overXP = x + 1 >= w->getXSize();
				bool overYP = y + 1 >= w->getYSize();
				bool overZP = z + 1 >= w->getZSize();
				bool overXN = x - 1 < 0;
				bool overYN = y - 1 < 0;
				bool overZN = z - 1 < 0;
				
				bool hasBlockXP = overXP ? false : w->getBlockBehavior(x + 1, y, z) != nullptr;
				bool hasBlockYP = overYP ? false : w->getBlockBehavior(x, y + 1, z) != nullptr;
				bool hasBlockZP = overZP ? false : w->getBlockBehavior(x, y, z + 1) != nullptr;
				bool hasBlockXN = overXN ? false : w->getBlockBehavior(x - 1, y, z) != nullptr;
				bool hasBlockYN = overYN ? false : w->getBlockBehavior(x, y - 1, z) != nullptr;
				bool hasBlockZN = overZN ? false : w->getBlockBehavior(x, y, z - 1) != nullptr;
				bool hiddenBlock = hasBlockXP && hasBlockYP && hasBlockZP && hasBlockXN && hasBlockYN && hasBlockZN;
				if (hiddenBlock) {
					continue;
				}
				if (!hasBlockYP) {
					genTopPlane(ob, mb, x*2, y * 2, z * 2, size);
				}
				if (!hasBlockYN) {
					genBottomPlane(ob, mb, x * 2, y * 2, z * 2, size);
				}
				if (!hasBlockXP) {
					genRightPlane(ob, mb, x * 2, y * 2, z * 2, size);
				}
				if (!hasBlockXN) {
					genLeftPlane(ob, mb, x * 2, y * 2, z * 2, size);
				}
				if (!hasBlockZP) {
					genFrontPlane(ob, mb, x * 2, y * 2, z * 2, size);
				}
				if (!hasBlockZN) {
					genBackPlane(ob, mb, x * 2, y * 2, z * 2, size);
				}
			}
		}
	}

	std::ofstream ofs(outputFile);
	if (!ofs.fail()) {
		ob.write(ofs);
	}
	ofs.close();
}

void PlayScene::exportBmp(const std::string & outputFile) {
	File::remove(outputFile);
	auto w = planet->getWorld();
	BlockTable blockTable = planet->getBlockTable();
	Terrain terrain = blockTable.getTerrain();
	std::vector<unsigned char> pixelVec = terrain.toPixelVec();
	int err = SOIL_save_image(outputFile.c_str(), SOIL_SAVE_TYPE_BMP, w->getXSize(), w->getZSize(), 4, pixelVec.data());
	if (err == 0) {
		std::cout << "export failed." << std::endl;
	}
}

void PlayScene::genTopPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Top", x, y, z);

	ob.newModel(buf)
		.vertex(glm::vec3(-size.x, 0, size.z) + glm::vec3(x,y,z))
		.vertex(glm::vec3(size.x, 0, size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(-size.x, 0, -size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(size.x, 0, -size.z) + glm::vec3(x, y, z))
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
}

void PlayScene::genBottomPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Bottom", x, y, z);
	ob.newModel(buf)
		.vertex(glm::vec3(size.x, -2, size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(-size.x, -2, size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(size.x, -2, -size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(-size.x, -2, -size.z) + glm::vec3(x, y, z))
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
}

void PlayScene::genLeftPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Left", x, y, z);
	ob.newModel(buf)
		.vertex(glm::vec3(-size.x, 0, -size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(-size.x, -2, -size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(-size.x, 0, size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(-size.x, -2, size.z) + glm::vec3(x, y, z))
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
}

void PlayScene::genRightPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Right", x, y, z);
	ob.newModel(buf)
		.vertex(glm::vec3(size.x, 0, size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(size.x, -2, size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(size.x, 0, -size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(size.x, -2, -size.z) + glm::vec3(x, y, z))
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
}

void PlayScene::genFrontPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Front", x, y, z);
	ob.newModel(buf)
		.vertex(glm::vec3(size.x, 0, -size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(size.x, -2, -size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(-size.x, 0, -size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(-size.x, -2, -size.z) + glm::vec3(x, y, z))
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
}

void PlayScene::genBackPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Back", x, y, z);
	ob.newModel(buf)
		.vertex(glm::vec3(-size.x, 0, size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(-size.x, -2, size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(size.x, 0, size.z) + glm::vec3(x, y, z))
		.vertex(glm::vec3(size.x, -2, size.z) + glm::vec3(x, y, z))
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
}

// private
NameSet PlayScene::createPlaneNameSet() {
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

NameSet PlayScene::createGunNameSet() {
        NameSet nameSet;
        nameSet.shader = "ModelTexture";
        nameSet.camera = "Block";
        nameSet.material = "Block";
        return nameSet;
}
}  // namespace planet
