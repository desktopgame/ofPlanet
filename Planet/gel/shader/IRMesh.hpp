#pragma once
#ifndef GEL_SHADER_IRMESH_HPP
#define GEL_SHADER_IRMESH_HPP
#include <memory>
#include <vector>
#include "../device/ITexture.hpp"

namespace gel {
class IRMaterial;
class IRShape;

class IRMesh {
       public:
        explicit IRMesh(const std::string& name);
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

       private:
        std::string name;
        std::vector<std::shared_ptr<IRMesh> > children;
        std::vector<std::shared_ptr<IRMaterial> > materials;
        std::shared_ptr<IRShape> shape;
        std::vector<std::shared_ptr<ITexture> > textures;
};
}  // namespace gel
#endif