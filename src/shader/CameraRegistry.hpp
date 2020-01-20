#pragma once
#ifndef SHADER_CAMERAREGISTRY_HPP
#define SHADER_CAMERAREGISTRY_HPP
#include <ofVec2f.h>

#include <memory>
#include <string>
#include <unordered_map>
namespace planet {

class Camera;
/**
 * CameraRegistry は、カメラを名前と紐付けて管理するレジストリです。
 */
class CameraRegistry {
       public:
		/**
		 * 指定の名前に紐付けられるカメラを返します。
		 * @param name
		 * @return
		 */
        static std::shared_ptr<Camera> create(const std::string& name);

		/**
		 * 指定の名前に紐付けられたカメラを返します。
		 * @param name
		 * @return
		 */
        static std::shared_ptr<Camera> get(const std::string& name);

		/**
		 * 指定の名前に紐付けられたカメラが存在するならそれを返し、
		 * しないなら作成して返します。
		 * @param name
		 * @return
		 */
		static std::shared_ptr<Camera> ref(const std::string& name);

		/**
		 * 指定の名前のカメラが存在するなら true を返します。
		 * @param name
		 * @return
		 */
        static bool has(const std::string& name);
		/**
		 * メインカメラを返します。
		 * @return
		 */
		static std::shared_ptr<Camera> main();
		/**
		 * デフォルトの画面サイズを設定します。
		 * @param defaultScreenSize
		 * @param forceUpdate
		 */
        static void setDefaultScreenSize(glm::vec2 defaultScreenSize,
                                         bool forceUpdate);
		/**
		 * デフォルトの画面サイズを返します。
		 * @return
		 */
        static glm::vec2 getDefaultScreenSize();

       private:
        static std::unordered_map<std::string, std::shared_ptr<Camera> >
            cameraMap;
        static glm::vec2 defaultScreenSize;
        CameraRegistry() = delete;
        ~CameraRegistry() = delete;
};
}  // namespace planet
#endif