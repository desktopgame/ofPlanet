#ifndef GEL_DEVICE_FBXMESHINFO_HPP
#define GEL_DEVICE_FBXMESHINFO_HPP
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "FbxMaterial.hpp"
#include "PngTexture.hpp"
namespace gel {
class FbxMeshInfo : public std::enable_shared_from_this<FbxMeshInfo> {
       public:
        FbxMeshInfo();
        void addChild(const std::shared_ptr<FbxMeshInfo> info);
        std::shared_ptr<FbxMeshInfo> getChildAt(int index) const;
        int getChildCount() const;

        void addVertex(const glm::vec4 vertex);
        glm::vec4 getVertexAt(int index) const;
        int getVertexCount() const;

        void addUV(const glm::vec2 uv);
        glm::vec2 getUVAt(int index) const;
        int getUVCount() const;

        void addNormal(const glm::vec4 normal);
        glm::vec4 getNormalAt(int index) const;
        int getNormalCount() const;

        void addIndex(int index);
        int getIndexAt(int index) const;
        int getIndexCount() const;

        void addTexture(const std::shared_ptr<PngTexture>& texture);
        std::shared_ptr<PngTexture> getTextureAt(int index);
        int getTextureCount();

        void addMaterial(const FbxMaterial& material);
        FbxMaterial& getMaterialAt(int index);
        int getMaterialCount() const;

       private:
        std::vector<std::shared_ptr<FbxMeshInfo> > children;
        std::weak_ptr<FbxMeshInfo> parent;
        std::vector<std::shared_ptr<PngTexture> > textureBuf;
        std::vector<glm::vec4> vertexBuf;
        std::vector<glm::vec4> normalBuf;
        std::vector<FbxMaterial> materialBuf;
        std::vector<glm::vec2> uvBuf;
        std::vector<int> indexBuf;
        std::vector<int> texIdBuf;
};
}  // namespace gel
#endif