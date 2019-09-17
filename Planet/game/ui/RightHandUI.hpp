#pragma once
#ifndef GAME_UI_RIGHTHAND_UI_HPP
#define GAME_UI_RIGHTHAND_UI_HPP
#include <glm/glm.hpp>
#include <memory>
#include "../../gel/asset/IModel.hpp"
#include "../../gel/gel.hpp"
#include "../../gel/shader/FontTable.hpp"
#include "../../gel/shader/Sprite.hpp"
#include "../../gel/signal/Signal.hpp"
class RightHandUI {
       public:
        explicit RightHandUI();
        void reset();
        void update();
        void draw(std::weak_ptr<gel::Camera> cameraRef);
		gel::Signal<>& onStartAnimation();
		gel::Signal<>& onEndAnimation();

		bool isAnimationNow() const;
		float getAnimationProgress01() const;

       private:
        void batch(std::weak_ptr<gel::Camera> cameraRef);
        glm::vec3 startGPos;
        glm::vec3 gPos;
        glm::vec3 gRot;
        std::shared_ptr<gel::IModel> tModel;
        gel::Timer clickTimer;
		gel::Signal<> startAnimationSignal;
		gel::Signal<> endAnimationSignal;
        bool clicked;
        gel::ScreenBuffer gunScrBuffer;
        bool gunCache;
};
#endif