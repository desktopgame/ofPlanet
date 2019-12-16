#include "Camera.hpp"

#include "../common/GLM.hpp"

#include "CameraObserver.hpp"
namespace planet {

Camera::Camera()
    : dirty(false),
      fov(30),
      nearPlane(1),
      farPlane(1000),
      screenSize(),
      position(0, 0, -1),
      lookAt(0, 0, 0),
      up(0, -1, 0),
      projectionMatrix(1.0f),
      viewMatrix(1.0f) {}

void Camera::setFOV(float fov) {
        this->dirty = true;
        this->fov = fov;
}

float Camera::getFOV() const { return fov; }

void Camera::setNearPlane(float nearPlane) {
        this->dirty = true;
        this->nearPlane = nearPlane;
}

float Camera::getNearPlane() const { return nearPlane; }

void Camera::setFarPlane(float farPlane) {
        this->dirty = true;
        this->farPlane = farPlane;
}

float Camera::getFarPlane() const { return farPlane; }

void Camera::setScreenSize(const glm::vec2& screenSize) {
        this->dirty = true;
        this->screenSize = screenSize;
}

glm::vec2 Camera::getScreenSize() const { return screenSize; }

void Camera::setPosition(const glm::vec3& position) {
        this->dirty = true;
        this->position = position;
}

glm::vec3 Camera::getPosition() const { return position; }

void Camera::setLookAt(const glm::vec3& lookAt) {
        this->dirty = true;
        this->lookAt = lookAt;
}

glm::vec3 Camera::getLookAt() const { return lookAt; }

void Camera::setUp(const glm::vec3& up) {
        this->dirty = true;
        this->up = up;
}

glm::vec3 Camera::getUp() const { return up; }

bool Camera::rehash() {
        if (!dirty) {
                return false;
        }
        this->dirty = false;
        this->projectionMatrix = glm::perspective(
            fov, screenSize.x / screenSize.y, nearPlane, farPlane);
        this->viewMatrix = glm::lookAt(position, lookAt, up);
        for (auto observerRef : observers) {
                auto observer = observerRef.lock();
                if (observer) {
                        observer->onRehash(shared_from_this());
                }
        }
        return true;
}

glm::mat4 Camera::getProjectionMatrix() const {
        if (dirty) {
                throw std::logic_error("camera is updated.");
        }
        return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() const {
        if (dirty) {
                throw std::logic_error("camera is updated.");
        }
        return viewMatrix;
}

glm::mat4 Camera::computeNormalMatrix(const glm::mat4 model) const {
        glm::mat4 normalMatrix = getViewMatrix() * model;
        normalMatrix = glm::inverse(normalMatrix);
        normalMatrix = glm::transpose(normalMatrix);
        return normalMatrix;
}

void Camera::apply(ofEasyCam& easyCam) {
        easyCam.setFov(fov);
        easyCam.setNearClip(nearPlane);
        easyCam.setFarClip(farPlane);
        easyCam.setPosition(position);
        easyCam.lookAt(lookAt, up);
        easyCam.setAspectRatio(screenSize.x / screenSize.y);
}

void Camera::apply(ofCamera& camera) {
        camera.setFov(fov);
        camera.setNearClip(nearPlane);
        camera.setFarClip(farPlane);
        camera.setPosition(position);
        camera.lookAt(lookAt, up);
        camera.setAspectRatio(screenSize.x / screenSize.y);
}

void Camera::addObserver(const std::weak_ptr<CameraObserver> observer) {
        observers.emplace_back(observer);
}
}  // namespace planet
