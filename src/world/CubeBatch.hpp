#pragma once
#ifndef WORLD_CUBEBATCH_HPP
#define WORLD_CUBEBATCH_HPP
#include <ofMatrix4x4.h>
#include <ofShader.h>

#include <array>
#include <memory>
#include <vector>
#include "Plane.hpp"
#include "GraphicsBatch.hpp"
namespace planet {

class Camera;
class CubeBatch : public GraphicsBatch {
       public:
        explicit CubeBatch(ofShader& shader, const glm::vec3& size);
        ~CubeBatch();

		void putFront(float x, float y, float z) override;
        void putBack(float x, float y, float z) override;
        void putLeft(float x, float y, float z) override;
        void putRight(float x, float y, float z) override;
        void putTop(float x, float y, float z) override;
        void putBottom(float x, float y, float z) override;
        void clear() override;
        void update() override;
        void render(GLuint texture) override;

       private:
		void put(PlaneType type, float x, float y, float z);
        void updatePlane(PlaneType type);
        std::vector<float>& getPosVec(PlaneType type);

        bool isInvalid;
        std::array<std::shared_ptr<Plane>, static_cast<int>(PlaneType::Count)>
            planes;
        std::array<std::vector<float>, static_cast<int>(PlaneType::Count)>
            posVec;
        ofBufferObject vbo[static_cast<int>(PlaneType::Count)];
		// ofShader& shader;
};
}  // namespace planet
#endif