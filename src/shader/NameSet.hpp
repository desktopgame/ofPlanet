#pragma once
#ifndef SHADER_NAMESET_HPP
#define SHADER_NAMESET_HPP
#include <memory>
#include <string>

#include "ShaderProperty.hpp"
namespace planet {

class Shader;
class Camera;
class Material;

class NameSet {
       public:
        explicit NameSet();

        std::string shader;
        std::string camera;
        std::string material;
        bool useOf;
        unsigned int texture;

        ShaderProperty aPosition;
        ShaderProperty aVertex;
        ShaderProperty aNormal;
        ShaderProperty aUV;
        ShaderProperty aTransformMatrix;

        ShaderProperty uMVPMatrix;
        ShaderProperty uModelMatrix;
        ShaderProperty uProjectionMatrix;
        ShaderProperty uViewMatrix;
        ShaderProperty uNormalMatrix;
        ShaderProperty uLightPos;

        ShaderProperty uTexture;
        ShaderProperty uAmbient;
        ShaderProperty uDiffuse;
        ShaderProperty uSpecular;
        ShaderProperty uShininess;
        ShaderProperty uSkybox;

        NameSet changeShader(const std::string& shader) const;
        NameSet changeCamera(const std::string& camera) const;
        NameSet changeMaterial(const std::string& material) const;

        NameSet togglePosition(bool enabled) const;
        NameSet toggleVertex(bool enabled) const;
        NameSet toggleNormal(bool enabled) const;
        NameSet toggleUV(bool enabled) const;
        NameSet toggleTransformMatrix(bool enabled) const;
        NameSet toggleMVPMatrix(bool enabled) const;
        NameSet toggleModelMatrix(bool enabled) const;
        NameSet toggleProjectionMatrix(bool enabled) const;
        NameSet toggleViewMatrix(bool enabled) const;
        NameSet toggleNormalMatrix(bool enabled) const;
        NameSet toggleLightPosMatrix(bool enabled) const;
        NameSet toggleTexture(bool enabled) const;
        NameSet toggleAmbient(bool enabled) const;
        NameSet toggleDiffuse(bool enabled) const;
        NameSet toggleSpecular(bool enabled) const;
        NameSet toggleShininess(bool enabled) const;
        NameSet toggleSkybox(bool enabled) const;

        std::shared_ptr<Shader> getShader() const;
        std::shared_ptr<Camera> getCamera() const;
        std::shared_ptr<Material> getMaterial() const;

        void apply(std::shared_ptr<Shader> shader) const;
        void applyCamera(std::shared_ptr<Shader> shader) const;
        void applyMaterial(std::shared_ptr<Shader> shader) const;

       private:
};
}  // namespace planet
#endif