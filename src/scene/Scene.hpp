#ifndef SCENE_ISCENE_HPP
#define SCENE_ISCENE_HPP
#include <memory>
#include <string>
namespace planet {
/**
 * Scene is unit of screen in game.
 */
class Scene : public std::enable_shared_from_this<Scene> {
       public:
        Scene();
        virtual ~Scene() = default;
        /**
         * show is called on switching to this scene.
         */
        virtual void show() = 0;

        /**
         * update is called when frames.
         */
        virtual void update() = 0;

        /**
         * draw is called when draw screen.
         */
        virtual void draw() = 0;

        /**
         * getNextScene is return a name of next scene.
         * @return
         */
        virtual std::string getNextScene() const = 0;

        /**
         * isFinished is return a whether a finished this scene.
         * @return
         */
        virtual bool isFinished() const = 0;

        /**
         * hide is called on switching to other scene from this screen.
         */
        virtual void hide() = 0;

       private:
};
}  // namespace planet
#endif