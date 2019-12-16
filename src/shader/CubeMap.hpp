#pragma once
#ifndef SHADER_CUBEMAPDESC_HPP
#define SHADER_CUBEMAPDESC_HPP
#include <ofTypes.h>
#include <ofVec3f.h>

#include <string>

#include "Buffer.hpp"
#include "CameraObserver.hpp"
#include "NameSet.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
namespace planet {

struct CubeMapDesc {
        unsigned char* right;
        unsigned char* left;
        unsigned char* top;
        unsigned char* bottom;
        unsigned char* front;
        unsigned char* back;

        explicit CubeMapDesc();
};

class CubeMap : public CameraObserver {
       public:
        explicit CubeMap(const CubeMapDesc& desc, const NameSet& nameSet,
                         const glm::vec3 scale, int width, const int height);
        ~CubeMap();
        void draw();
        void onRehash(std::shared_ptr<const Camera> camera) override;

       private:
        static GLuint loadCubeMap(const CubeMapDesc& desc, const int width,
                                  const int height);
        GLuint texture;
        NameSet nameSet;
        VertexArray vao;
        VertexBuffer<GL_DYNAMIC_DRAW> vertex;
        glm::vec3 scale;
};
}  // namespace planet
#endif