#include "ModelTexture.hpp"

#include "../common/GLM.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "ShaderRegistry.hpp"
namespace planet {

ModelTexture::ModelTexture(const NameSet& nameSet)
    : transform(),
      nameSet(nameSet),
      ofFBO(),
      screenBuffer(spriteNameSet(nameSet), ofGetWidth(), ofGetHeight()),
      modelLoader(),
      light() {
        ofFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}

ModelTexture::~ModelTexture() {}

void ModelTexture::load(const std::string& path) {
        bool loaded = modelLoader.loadModel(path);
        assert(loaded);
}

void ModelTexture::drawToBuffer() {
        auto camera = nameSet.getCamera();

        // ofFBO.begin();
        screenBuffer.bind();
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO,
                            GL_ONE);
        glEnable(GL_DEPTH_TEST);
        // ofClear(0);
        ofFBO.clear();
        ofEnableAlphaBlending();
        ofSetBackgroundColor(0, 0, 0, 0);
        // ofClear(0);

        light.enable();
        light.setPointLight();
        light.setPosition(transform.position);
        light.setAmbientColor(ofFloatColor(0.1, 0.1, 0.5, 1.0));
        light.setDiffuseColor(ofFloatColor(0.2, 0.2, 1.0));
        light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));

        ofPushMatrix();
        ofTranslate(transform.position);
        ofRotateY(transform.rotation.y);
        ofRotateX(transform.rotation.x);
        ofRotateZ(transform.rotation.z);
        ofScale(transform.scale);
        modelLoader.draw(ofPolyRenderMode::OF_MESH_FILL);
        light.disable();
        ofPopMatrix();

        // ofFBO.end();
        screenBuffer.unbind();
}

void ModelTexture::render() {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // ofFBO.draw(0,0,ofGetWidth(), ofGetHeight());
        screenBuffer.render();
}

Transform& ModelTexture::getTransform() { return transform; }

const Transform& ModelTexture::getTransform() const { return transform; }

NameSet ModelTexture::spriteNameSet(const NameSet& nameSet) {
        NameSet ns = nameSet;
        ns.shader = "Sprite";
        return ns;
}
}  // namespace planet
