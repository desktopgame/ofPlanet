#pragma once
#ifndef WORLD_CUBERENDERER_HPP
#define WORLD_CUBERENDERER_HPP
#include <unordered_map>
#include <ofShader.h>

#include "CubeBatch.hpp"
#include "GraphicsRenderer.hpp"
namespace planet {

class Camera;
class CubeRenderer : public GraphicsRenderer {
       public:
        explicit CubeRenderer(ofShader& shader, const glm::vec3& size);
       protected:
		std::shared_ptr<GraphicsBatch> createBatch(ofShader& shader) override;
       private:
		glm::vec3 size;
};
}  // namespace planet
#endif