#pragma once
#ifndef SHADER_MODELTEXTURE_HPP
#define SHADER_MODELTEXTURE_HPP
#include <ofEasyCam.h>
#include <ofFbo.h>
#include <ofLight.h>
#include <ofVec3f.h>
#include <ofxAssimpModelLoader.h>

#include <string>

#include "NameSet.hpp"
#include "ScreenBuffer.hpp"
#include "Transform.hpp"
namespace planet {

class ModelTexture {
       public:
        explicit ModelTexture(const NameSet& nameSet);
        ~ModelTexture();

        void load(const std::string& path);
        void drawToBuffer();
        void render();

        Transform& getTransform();
        const Transform& getTransform() const;

       private:
        static NameSet spriteNameSet(const NameSet& nameSet);
        Transform transform;
        NameSet nameSet;
        ofLight light;
        ofFbo ofFBO;
        ScreenBuffer screenBuffer;
        ofxAssimpModelLoader modelLoader;
};
}  // namespace planet
#endif