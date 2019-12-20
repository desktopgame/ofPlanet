#pragma once
#ifndef GAME_PLAYSCENE_HPP
#define GAME_PLAYSCENE_HPP
#include <ofEasyCam.h>
#include <ofShader.h>
#include <ofxImGui.h>
#include <vector>

#include <memory>

#include "../common/Random.hpp"
#include "../common/StateValue.hpp"
#include "../common/FirstPersonController.hpp"
#include "../common/Timer.hpp"
#include "../scene/Scene.hpp"
#include "../shader/ModelTexture.hpp"
#include "../shader/Plane.hpp"
#include "../shader/Transform.hpp"
#include "../world/Planet.hpp"
#include "../world/World.hpp"
#include "../world/biome/Biome.hpp"
#include "../imguiex/ListBox.hpp"
#include "../imguiex/CheckBox.hpp"
#include "../imguiex/RadioButtonGroup.hpp"
#include "../imguiex/InputField.hpp"
namespace planet {
class PlayScene : public Scene {
       public:
        explicit PlayScene();
        ~PlayScene();
        void show() override;
        void update() override;
        void draw() override;
        void hide() override;

        std::string getNextScene() const override;
        bool isFinished() const override;

       private:
		void playUpdate();
		void playDraw();

        ofEasyCam cam;
        std::shared_ptr<Planet> planet;
		Random rand;
		float cameraAngle;

		ofxImGui::Gui gui;
		glm::vec3 worldSize;
		float cameraSpeed;
		StateValue<bool> playMode;
		FirstPersonController fpsCon;
		std::vector<std::shared_ptr<Biome> > biomes;
		imguiex::ListBox biomeNames;
		imguiex::RadioButtonGroup exportTypes;
		imguiex::InputField<> exportFile;

        static NameSet createPlaneNameSet();
        static NameSet createGunNameSet();
};
}  // namespace planet
#endif