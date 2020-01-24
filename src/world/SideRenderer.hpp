#pragma once
#ifndef WORLD_SIDERENDERER_HPP
#define WORLD_SIDERENDERER_HPP
#include <ofMatrix4x4.h>
#include <ofShader.h>

#include <array>
#include <memory>
#include <vector>

#include "Plane.hpp"
namespace planet {

class Camera;
class SideRenderer {
       public:
        explicit SideRenderer(ofShader& shader);
        ~SideRenderer();

        void put(PlaneType type, float x, float y, float z);
        void putFront(float x, float y, float z);
        void putBack(float x, float y, float z);
        void putLeft(float x, float y, float z);
        void putRight(float x, float y, float z);
        void putTop(float x, float y, float z);
        void putBottom(float x, float y, float z);
        void clear();
        void updatePlane();
		void updateCamera(Camera& camera);
        void render(GLuint texture);

       private:
        void updatePlane(PlaneType type);
        std::vector<float>& getPosVec(PlaneType type);

        bool isInvalid;
        std::array<std::shared_ptr<Plane>, static_cast<int>(PlaneType::Count)>
            planes;
        std::array<std::vector<float>, static_cast<int>(PlaneType::Count)>
            posVec;
        ofBufferObject vbo[static_cast<int>(PlaneType::Count)];
		ofShader& shader;
};
}  // namespace planet
#endif