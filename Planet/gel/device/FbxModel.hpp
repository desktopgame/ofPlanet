#ifndef GEL_DEVICE_FBXMODEL_HPP
#define GEL_DEVICE_FBXMODEL_HPP
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../gli.hpp"
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
        void drawMeshInfo(std::shared_ptr<FbxMeshInfo> meshInfo);
        std::shared_ptr<FbxMeshInfo> procRecNode(FbxNode* node);
        std::shared_ptr<FbxMeshInfo> procRecNode(FbxNode* node, int depth);
        void procNode(FbxNode* node, std::shared_ptr<FbxMeshInfo> target);
        void procVertex(FbxNode* node, std::shared_ptr<FbxMeshInfo> target);
        void procIndex(FbxNode* node, std::shared_ptr<FbxMeshInfo> target);
        void procNormal(FbxNode* node, std::shared_ptr<FbxMeshInfo> target);
        void procUV(FbxNode* node, std::shared_ptr<FbxMeshInfo> target);
        void procMaterial(FbxNode* node, std::shared_ptr<FbxMeshInfo> target);
        void procSide(FbxNode* node, std::shared_ptr<FbxMeshInfo> target);
        void applyShader(FbxMaterial& mat);
        void applyTriangle(FbxMaterial& mat);
        void applyQuad(FbxMaterial& mat);

        FbxManager* fbxManager;
        FbxScene* fbxScene;
        FbxImporter* fbxImporter;
        std::string textureShaderName;
        std::string colorShaderName;
        NameRule nameRule;
        std::shared_ptr<FbxMeshInfo> meshInfo;
        AABB aabb;
};
}  // namespace gel
#endif