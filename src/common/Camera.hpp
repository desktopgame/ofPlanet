#pragma once
#ifndef SHADER_CAMERA_HPP
#define SHADER_CAMERA_HPP
#include <ofEasyCam.h>
#include <ofMatrix4x4.h>
#include <ofTypes.h>
#include <ofVec2f.h>
#include <ofVec3f.h>

#include <memory>
#include <vector>
namespace planet {

/**
 * Camera はカメラの基本的な行列を計算するクラスです。
 */
class Camera : public std::enable_shared_from_this<Camera> {
       public:
        explicit Camera();

        /**
         * Field Of View を設定します。
         * @param fov
         */
        void setFOV(float fov);
        /**
         * Field Of View を返します。
         * @return
         */
        float getFOV() const;

        /**
         * ニアプレーンを設定します。
         * @param nearPlane
         */
        void setNearPlane(float nearPlane);
        /**
         * ニアプレーンを返します。
         * @return
         */
        float getNearPlane() const;

        /**
         * ファープレーンを設定します。
         * @param farPlane
         */
        void setFarPlane(float farPlane);
        /**
         * ファープレーンを返します。
         * @return
         */
        float getFarPlane() const;

        /**
         * 画面サイズを設定します。
         * @param screenSize
         */
        void setScreenSize(const glm::vec2& screenSize);
        /**
         * 画面サイズを返します。
         * @retunr
         */
        glm::vec2 getScreenSize() const;

        /**
         * カメラの位置を設定します。
         * @param position
         */
        void setPosition(const glm::vec3& position);
        /**
         * カメラの位置を返します。
         * @return
         */
        glm::vec3 getPosition() const;

        /**
         * カメラの注視点を設定します。
         * @param lookAt
         */
        void setLookAt(const glm::vec3& lookAt);
        /**
         * カメラの注視点を返します。
         * @return
         */
        glm::vec3 getLookAt() const;

        /**
         * カメラの上方向を設定します。
         * @param up
         */
        void setUp(const glm::vec3& up);
        /**
         * カメラの上方向を返します。
         * @return
         */
        glm::vec3 getUp() const;

        /**
         * 行列を再計算します。
         * @return
         */
        bool rehash();
        /**
         * プロジェクション行列を返します。
         * @return
         */
        glm::mat4 getProjectionMatrix() const;
        /**
         * ビュー行列を返します。
         */
        glm::mat4 getViewMatrix() const;
        /**
         * ノーマル行列を返します。
         * @param model
         * @return
         */
        glm::mat4 computeNormalMatrix(const glm::mat4 model) const;

       private:
        bool dirty;
        float fov;
        float nearPlane;
        float farPlane;
        glm::vec2 screenSize;
        glm::vec3 position;
        glm::vec3 lookAt;
        glm::vec3 up;

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
};
}  // namespace planet
#endif