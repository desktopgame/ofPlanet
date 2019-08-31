#include "Camera.hpp"
#include <algorithm>
#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/raw_data.hpp>
#include <stdexcept>
#include "../Game.hpp"
#include "../gli.hpp"
namespace gel {
Camera::Camera() : transform(), screenWidth(0), screenHeight(0) {}

void Camera::calculate() {
        assert(screenWidth != 0);
        assert(screenHeight != 0);
        glViewport(0, 0, screenWidth, screenHeight);
        auto pos = transform.position;
        auto lookat = pos + transform.forward();
        this->projection =
            glm::perspective(30.0f, screenWidth / screenHeight, 1.0f, 1000.0f);
        // this->view = playerCamera.calculate();
        this->view = glm::lookAt(pos, lookat, glm::vec3(0, -1, 0));
        this->inverseView = glm::lookAt(lookat, pos, glm::vec3(0, -1, 0));
        this->model = glm::mat4(1.0f);
        this->mvp = projection * view * model;
        this->normalMatrix = view * model;
        this->normalMatrix = glm::inverse(normalMatrix);
        this->normalMatrix = glm::transpose(normalMatrix);
        this->ortho = glm::ortho(0.0f, static_cast<GLfloat>(this->screenWidth),
                                 static_cast<GLfloat>(this->screenHeight), 0.0f,
                                 -1.0f, 1.0f);
}

glm::mat4 Camera::getProjection() const { return projection; }

glm::mat4 Camera::getView() const { return view; }

glm::mat4 Camera::getInverseView() const { return inverseView; }

glm::mat4 Camera::getModel() const { return model; }

glm::mat4 Camera::getMVP() const { return mvp; }

glm::mat4 Camera::getNormal() const { return normalMatrix; }

glm::mat4 Camera::getOrtho() const { return ortho; }
}  // namespace gel