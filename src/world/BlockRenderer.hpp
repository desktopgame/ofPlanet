#pragma once
#ifndef WORLD_BLOCKRENDERER_HPP
#define WORLD_BLOCKRENDERER_HPP
#include <unordered_map>
#include <ofShader.h>

#include "SideRenderer.hpp"
namespace planet {

class Camera;
class BlockRenderer {
       public:
        explicit BlockRenderer(ofShader& shader);

        void put(GLuint texture, PlaneType type, float x, float y, float z);
        void putFront(GLuint texture, float x, float y, float z);
        void putBack(GLuint texture, float x, float y, float z);
        void putLeft(GLuint texture, float x, float y, float z);
        void putRight(GLuint texture, float x, float y, float z);
        void putTop(GLuint texture, float x, float y, float z);
        void putBottom(GLuint texture, float x, float y, float z);
        void clear();
        void updatePlane();
		void updateCamera(Camera& camera);
        void render();

       private:
        std::shared_ptr<SideRenderer> ref(GLuint texture);
        std::unordered_map<GLuint, std::shared_ptr<SideRenderer> > map;
		ofShader& shader;
};
}  // namespace planet
#endif