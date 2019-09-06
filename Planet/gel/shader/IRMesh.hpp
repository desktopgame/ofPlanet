#pragma once
#ifndef GEL_SHADER_IRMESH_HPP
#define GEL_SHADER_IRMESH_HPP
#include <memory>
#include <vector>
#include "../device/ITexture.hpp"

namespace gel {
class IRMaterial;
class IRShape;
class IRModel;

class IRMesh : public std::enable_shared_from_this<IRMesh> {
       public:
        explicit IRMesh(const std::weak_ptr<IRModel>& model,
                        const std::string& name);
        ~IRMesh();
        std::shared_ptr<IRMesh> addMesh(const std::shared_ptr<IRMesh>& mesh);
        std::shared_ptr<IRMesh> addMesh(const std::string& name);
        std::shared_ptr<IRMesh> getMesh(int index) const;
        void removeMesh(int index);
        size_t getMeshCount() const;

        std::shared_ptr<IRMaterial> addMaterial(
            const std::shared_ptr<IRMaterial>& material);
        std::shared_ptr<IRMaterial> addMaterial();
        std::shared_ptr<IRMaterial> getMaterial(int index) const;
        void removeMaterial(int index);
        size_t getMaterialCount() const;

        std::shared_ptr<ITexture> addTexture(
            const std::shared_ptr<ITexture>& texture);
        std::shared_ptr<ITexture> getTexture(int index) const;
        void removeTexture(int index);
        size_t getTextureCount() const;

        std::string getName() const;
        std::shared_ptr<IRShape> getShape() const;

        void setLocalPosition(const glm::vec4 position);
        glm::vec4 getLocalPosition() const;

        void setLocalRotation(const glm::vec4 rotation);
        glm::vec4 getLocalRotation() const;

        void setLocalScale(const glm::vec4 scale);
        glm::vec4 getLocalScale() const;

        glm::mat4 getLocalMatrix() const;
        glm::mat4 getTreeMatrix() const;

        std::weak_ptr<IRMesh> getParent() const;

        std::weak_ptr<IRModel> getModel() const;

       private:
        std::weak_ptr<IRModel> model;
        std::weak_ptr<IRMesh> parent;
        std::string name;
        std::vector<std::shared_ptr<IRMesh> > children;
        std::vector<std::shared_ptr<IRMaterial> > materials;
        std::shared_ptr<IRShape> shape;
        std::vector<std::shared_ptr<ITexture> > textures;
        glm::vec4 position;
        glm::vec4 rotation;
        glm::vec4 scale;
};
}  // namespace gel
#endif