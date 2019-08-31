#ifndef GEL_GSYSTEM_CAMERA_HPP
#define GEL_GSYSTEM_CAMERA_HPP
#include <glm/glm.hpp>
#include "Transform.hpp"
namespace gel {
class Camera {
       public:
        Camera();
        void calculate();
        float screenWidth;
        float screenHeight;
        Transform transform;

        glm::mat4 getProjection() const;
        glm::mat4 getView() const;
        glm::mat4 getInverseView() const;
        glm::mat4 getModel() const;
        glm::mat4 getMVP() const;
        glm::mat4 getNormal() const;
        glm::mat4 getOrtho() const;

       private:
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
        glm::mat4 mvp;
        glm::mat4 ortho;
        glm::mat4 inverseView;
        glm::mat4 normalMatrix;
};
}  // namespace gel
#endif