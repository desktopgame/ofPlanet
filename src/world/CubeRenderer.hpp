#pragma once
#ifndef WORLD_CUBERENDERER_HPP
#define WORLD_CUBERENDERER_HPP
#include <unordered_map>
#include <ofShader.h>

#include "CubeBatch.hpp"
namespace planet {

class Camera;
class CubeRenderer {
       public:
        explicit CubeRenderer(ofShader& shader);

        void put(GLuint texture, PlaneType type, float x, float y, float z);
        void putFront(GLuint texture, float x, float y, float z);
        void putBack(GLuint texture, float x, float y, float z);
        void putLeft(GLuint texture, float x, float y, float z);
        void putRight(GLuint texture, float x, float y, float z);
        void putTop(GLuint texture, float x, float y, float z);
        void putBottom(GLuint texture, float x, float y, float z);
        void clear();
        void updatePlane();
        void render();

       private:
        std::shared_ptr<CubeBatch> ref(GLuint texture);
        std::unordered_map<GLuint, std::shared_ptr<CubeBatch> > map;
		ofShader& shader;
};
}  // namespace planet
#endif