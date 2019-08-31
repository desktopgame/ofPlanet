#ifndef GEL_SCENE_SCENEMANAGER_HPP
#define GEL_SCENE_SCENEMANAGER_HPP
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace gel {
class IScene;
/**
 * SceneManager is manage a `IScene`.
 * this class is can to transition of scene to scene.
 */
class SceneManager {
       public:
        SceneManager();
        /**
         * be scene mapping by name.
         * @param name
         * @param scene
         */
        void put(const std::string& name, const std::shared_ptr<IScene>& scene);
        /**
         * remove a mapping.
         * @param name
         */
        void remove(const std::string& name);
        /**
         * set a current scene.
         * @param name
         */
        void bind(const std::string& name);
        /**
         * update a current scene.
         * be transition if finished current scene.
         */
        void update();
        /**
         * draw a current scene.
         */
        void draw();
        /**
         * return a current scene.
         * @return
         */
        std::optional<std::shared_ptr<IScene> > getScene();

       private:
        std::string mCurrent;
        std::unordered_map<std::string, std::shared_ptr<IScene> > mSceneMap;
};
}  // namespace gel
#endif