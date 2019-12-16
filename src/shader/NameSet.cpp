#include "NameSet.hpp"

#include "../common/GLM.hpp"
#include "Camera.hpp"
#include "CameraRegistry.hpp"
#include "Material.hpp"
#include "MaterialRegistry.hpp"
#include "Shader.hpp"
#include "ShaderRegistry.hpp"
namespace planet {

NameSet::NameSet()
    : shader(""),
      camera(""),
      material(""),
      useOf(false),
      texture(0),
      aPosition("aPosition", false),
      aVertex("aVertex", false),
      aNormal("aNormal", false),
      aUV("aUV", false),
      aTransformMatrix("aTransformMatrix", false),
      uMVPMatrix("uMVPMatrix", false),
      uModelMatrix("uModelMatrix", false),
      uProjectionMatrix("uProjectionMatrix", false),
      uViewMatrix("uViewMatrix", false),
      uNormalMatrix("uNormalMatrix", false),
      uLightPos("uLightPos", false),
      uTexture("uTexture", false),
      uAmbient("uAmbient", false),
      uDiffuse("uDiffuse", false),
      uSpecular("uSpecular", false),
      uShininess("uShininess", false),
      uSkybox("uSkybox", false) {}

std::shared_ptr<Shader> NameSet::getShader() const {
        return ShaderRegistry::get(shader);
}

std::shared_ptr<Camera> NameSet::getCamera() const {
        return CameraRegistry::get(camera);
}

std::shared_ptr<Material> NameSet::getMaterial() const {
        return MaterialRegistry::get(material);
}

void NameSet::apply(std::shared_ptr<Shader> shader) const {
        applyMaterial(shader);
        applyCamera(shader);
}

void NameSet::applyCamera(std::shared_ptr<Shader> shader) const {
        auto mat = getMaterial();
        auto cam = getCamera();
        // matrix
        if (this->uMVPMatrix.enabled) {
                shader->setUniformMatrix4f(
                    this->uMVPMatrix.name, 1, GL_FALSE,
                    (cam->getProjectionMatrix() * cam->getViewMatrix()));
        }
        if (this->uNormalMatrix.enabled) {
                shader->setUniformMatrix4f(
                    this->uNormalMatrix.name, 1, GL_FALSE,
                    (cam->computeNormalMatrix(glm::mat4(1.0f))));
        }
}

void NameSet::applyMaterial(std::shared_ptr<Shader> shader) const {
        auto mat = getMaterial();
        auto cam = getCamera();
        // material
        if (this->uTexture.enabled) {
                shader->setUniform1i(this->uTexture.name, this->texture);
        }
        if (this->uAmbient.enabled) {
                shader->setUniform4fv(this->uAmbient.name, 1,
                                      &(mat->ambient.x));
        }
        if (this->uDiffuse.enabled) {
                shader->setUniform4fv(this->uDiffuse.name, 1,
                                      &(mat->diffuse.x));
        }
        if (this->uSpecular.enabled) {
                shader->setUniform4fv(this->uSpecular.name, 1,
                                      &(mat->specular.x));
        }
        if (this->uShininess.enabled) {
                shader->setUniform1f(this->uShininess.name, mat->shininess);
        }
}

NameSet NameSet::changeShader(const std::string& shader) const {
        NameSet r = *this;
        r.shader = shader;
        return r;
}

NameSet NameSet::changeCamera(const std::string& camera) const {
        NameSet r = *this;
        r.camera = camera;
        return r;
}

NameSet NameSet::changeMaterial(const std::string& material) const {
        NameSet r = *this;
        r.material = material;
        return r;
}

NameSet NameSet::togglePosition(bool enabled) const {
        NameSet r = *this;
        r.aPosition.enabled = enabled;
        return r;
}

NameSet NameSet::toggleVertex(bool enabled) const {
        NameSet r = *this;
        r.aVertex.enabled = enabled;
        return r;
}

NameSet NameSet::toggleNormal(bool enabled) const {
        NameSet r = *this;
        r.aNormal.enabled = enabled;
        return r;
}

NameSet NameSet::toggleUV(bool enabled) const {
        NameSet r = *this;
        r.aUV.enabled = enabled;
        return r;
}

NameSet NameSet::toggleTransformMatrix(bool enabled) const {
        NameSet r = *this;
        r.aTransformMatrix.enabled = enabled;
        return r;
}

NameSet NameSet::toggleMVPMatrix(bool enabled) const {
        NameSet r = *this;
        r.uMVPMatrix.enabled = enabled;
        return r;
}

NameSet NameSet::toggleModelMatrix(bool enabled) const {
        NameSet r = *this;
        r.uModelMatrix.enabled = enabled;
        return r;
}

NameSet NameSet::toggleProjectionMatrix(bool enabled) const {
        NameSet r = *this;
        r.uProjectionMatrix.enabled = enabled;
        return r;
}

NameSet NameSet::toggleViewMatrix(bool enabled) const {
        NameSet r = *this;
        r.uViewMatrix.enabled = enabled;
        return r;
}

NameSet NameSet::toggleNormalMatrix(bool enabled) const {
        NameSet r = *this;
        r.uNormalMatrix.enabled = enabled;
        return r;
}

NameSet NameSet::toggleLightPosMatrix(bool enabled) const {
        NameSet r = *this;
        r.uLightPos.enabled = enabled;
        return r;
}

NameSet NameSet::toggleTexture(bool enabled) const {
        NameSet r = *this;
        r.uTexture.enabled = enabled;
        return r;
}

NameSet NameSet::toggleAmbient(bool enabled) const {
        NameSet r = *this;
        r.uAmbient.enabled = enabled;
        return r;
}

NameSet NameSet::toggleDiffuse(bool enabled) const {
        NameSet r = *this;
        r.uDiffuse.enabled = enabled;
        return r;
}

NameSet NameSet::toggleSpecular(bool enabled) const {
        NameSet r = *this;
        r.uSpecular.enabled = enabled;
        return r;
}

NameSet NameSet::toggleShininess(bool enabled) const {
        NameSet r = *this;
        r.uShininess.enabled = enabled;
        return r;
}

NameSet NameSet::toggleSkybox(bool enabled) const {
        NameSet r = *this;
        r.uSkybox.enabled = enabled;
        return r;
}
}  // namespace planet
