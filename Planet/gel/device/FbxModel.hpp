#ifndef GEL_DEVICE_FBXMODEL_HPP
#define GEL_DEVICE_FBXMODEL_HPP
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../gli.hpp"
#include "../shader/IRMaterial.hpp"
#include "../shader/IRModel.hpp"
#include "../shader/NameRule.hpp"
#include "IModel.hpp"
#include "PngTexture.hpp"
#include "fbxsdk.h"
namespace gel {
class FbxModel : public IModel {
       public:
        FbxModel(FbxManager* fbxManager, const std::string& textureShaderName,
                 const std::string& colorShaderName, const NameRule& nameRule);
        ~FbxModel() = default;
        void loadOnAsync(const std::string& path) override;
        void loadOnGLThread(const std::string& path) override;
        void unload() override;
        std::shared_ptr<IRModel> getIRModel() const override;

       private:
        void drawIR(std::shared_ptr<IRMesh> mesh);
        void procIRRec(FbxNode* node, std::shared_ptr<IRMesh> mesh, int depth);
        void procIR(FbxNode* node, std::shared_ptr<IRMesh> mesh, int depth);
        void procIRVertex(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRIndex(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRNormal(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRUV(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRMaterials(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRMaterial(FbxNode* node, std::shared_ptr<IRMesh> mesh,
                            FbxSurfaceMaterial* fbxMat);
        void procIRLambert(FbxNode* node, std::shared_ptr<IRMesh> mesh,
                           FbxSurfaceLambert* fbxMat);
        void procIRPhong(FbxNode* node, std::shared_ptr<IRMesh> mesh,
                         FbxSurfacePhong* fbxMat);
        void procIRSide(FbxNode* node, std::shared_ptr<IRMesh> mesh);
        void procIRTexture(FbxNode* node, std::shared_ptr<IRMesh> mesh,
                           FbxSurfaceMaterial* fbxMat,
                           std::shared_ptr<IRMaterial> irMat);
        bool hasMeshAttr(FbxNode* node) const;
        void indent(int depth) const;
        static FbxLayerElementNormal* findElementNormal(FbxMesh* mesh);
        static FbxLayerElementUV* findElementUV(FbxMesh* mesh);
        static FbxLayerElementMaterial* findElementMaterial(FbxMesh* mesh);

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