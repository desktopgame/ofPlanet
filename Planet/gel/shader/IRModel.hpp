#pragma once
#ifndef GEL_SHADER_IRMODEL_HPP
#define GEL_SHADER_IRMODEL_HPP
#include <glm/glm.hpp>
#include <memory>
#include "../util/SharedHelper.hpp"

namespace gel {
class IRMesh;

class IRModel : public std::enable_shared_from_this<IRModel> {
       public:
        template <typename T>
        friend class SharedHelper;

        static std::shared_ptr<IRModel> create();

        ~IRModel();
        std::shared_ptr<IRMesh> getMesh() const;

        void setModelMatrix(const glm::mat4 modelMatrix);
        glm::mat4 getModelMatrix() const;

        void setViewMatrix(const glm::mat4 viewMatrix);
        glm::mat4 getViewMatrix() const;

        void setProjectionMatrix(const glm::mat4 projectionMatrix);
        glm::mat4 getProjectionMatrix() const;

        glm::mat4 getNormalMatrix() const;

       private:
        explicit IRModel();
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        std::shared_ptr<IRMesh> mesh;
};
}  // namespace gel
#endif