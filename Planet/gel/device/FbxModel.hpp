#ifndef GEL_DEVICE_FBXMODEL_HPP
#define GEL_DEVICE_FBXMODEL_HPP
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../gli.hpp"
#include "../shader/IRModel.hpp"
#include "../shader/NameRule.hpp"
#include "FbxMaterial.hpp"
#include "FbxMeshInfo.hpp"
#include "IModel.hpp"
#include "PngTexture.hpp"
#include "fbxsdk.h"
namespace gel {
class FbxModel : public IModel {
       public:
        FbxModel(FbxManager* fbxManager, const std::string& textureShaderName,
                 const std::string& colorShaderName, const NameRule& nameRule);
        void load(const std::string& path) override;
        void unload(const std::string& path) override;
        void draw() override;
        AABB getAABB() const override;

       private:
        void drawIR(std::shared_ptr<IRMesh> mesh);
        void procIRRec(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIR(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRVertex(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRIndex(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRNormal(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRUV(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRMaterial(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRSide(FbxNode* node, std::shared_ptr<IRMesh> mesh);

        FbxManager* fbxManager;
        FbxScene* fbxScene;
        FbxImporter* fbxImporter;
        std::string textureShaderName;
        std::string colorShaderName;
        NameRule nameRule;
        std::shared_ptr<IRModel> model;
        AABB aabb;
};
}  // namespace gel
#endif