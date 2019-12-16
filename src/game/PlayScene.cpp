#include "PlayScene.hpp"

#include <ofImage.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <string>
#include <sstream>

#include "../common/glfw.hpp"
#include "../common/GLM.hpp"
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
#include "Screen.hpp"
namespace planet {
using ExportMode = enum {
	EXPORT_JSON = 0,
	EXPORT_OBJ,
};
PlayScene::PlayScene()
    : Scene(),
      planet(std::make_shared<Planet>(createPlaneNameSet())),
      rand(),
      cameraAngle(0),
      gui(),
      worldSize(128,64,128),
      biomeList(),
      biomeIndex(0),
      exportMode(EXPORT_JSON),
      exportPath("./data"),
      cameraSpeed(0.01f),
      playMode(false),
      fpsCon() {
	fpsCon.enable();
	fpsCon.setRotateSpeed(1.0f);
	fpsCon.setMoveSpeed(1.0f);
	fpsCon.setMode(FirstPersonController::Mode::Key);
	//biomeList[0] = "Plain";
	//biomeList[1] = "Hill";
	gui.setup();
	auto files = Directory::files("./data/script", false);
	this->biomeList = static_cast<char**>(std::malloc(sizeof(char*) * files.size()));
	for (int i = 0; i < static_cast<int>(files.size()); i++) {
		auto file = files.at(i);
		biomes.emplace_back(std::make_shared<planet::ScriptBiome>(file));
		biomeList[i] = _strdup(file.c_str());
	}
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
			cam->setPosition(glm::vec3(128, 64, 128));
			cam->setLookAt(glm::vec3(64, 0, 64));
			cam->rehash();
			w->setPlayMode(false);
			w->setDrawSkyBox(false);
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
	// update camera
	auto myCam = CameraRegistry::get("Block");
	if (this->playMode.get()) {
		fpsCon.update();
		if (playMode.testIsChanged()) {
			myCam->setPosition(glm::vec3(64,32,64));
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
		myCam->setPosition(glm::vec3(64.0f + (64.0f * cx), 64, 64.0f + (64.0f * cz)));
		myCam->setLookAt(glm::vec3(64, 0, 64));
		myCam->rehash();
		this->cameraAngle += cameraSpeed;
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
	// Setting Window
	ImGui::Begin("Setting");
	ImGui::DragFloat3("Size", &worldSize.x, 2);
	ImGui::ListBox("Biome", &biomeIndex, biomeList, IM_ARRAYSIZE(biomeList), 4);
	ImGui::DragFloat("CameraSpeed", &cameraSpeed, 0.001f);
	if (ImGui::Button("Generate") && biomeIndex >= 0 && !biomes.empty()) {
		planet->generate(worldSize, biomes.at(biomeIndex));
	}
	ImGui::Checkbox("PlayMode", &playMode.getNewValue());
	playMode.detect();
	ImGui::End();
	// Parameter Window
	ImGui::Begin("Parameter");
	biomes.at(biomeIndex)->onGUI();
	ImGui::End();
	// Exporter Window
	ImGui::Begin("Exporter");
	//[JSON] [OBJ]
	ImGui::RadioButton("JSON", &exportMode, EXPORT_JSON);
	ImGui::SameLine();
	ImGui::RadioButton("OBJ", &exportMode, EXPORT_OBJ);
	ImGui::InputText("File", exportPath, 255);
	if (ImGui::Button("Export")) {
		if (exportMode == EXPORT_JSON) {
			WorldIO::toJson(exportPath, planet->getWorld());
		} else if (exportMode == EXPORT_OBJ) {

		}
	}
	ImGui::End();
	gui.end();
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
