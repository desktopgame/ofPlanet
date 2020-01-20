#pragma once
#ifndef SHADER_CAMERAOBSERVER_HPP
#define SHADER_CAMERAOBSERVER_HPP
#include <memory>
namespace planet {

class Camera;
/**
 * CameraObserver は、カメラの変更を検出するリスナーです。
 */
class CameraObserver : public std::enable_shared_from_this<CameraObserver> {
       public:
        CameraObserver() = default;
        virtual ~CameraObserver() = default;
        /**
         * カメラが変更されると呼び出されます。
         * @param camera
         */
        virtual void onRehash(std::shared_ptr<const Camera> camera) = 0;

       private:
};
}  // namespace planet
#endif