#pragma once
#ifndef WORLD_ENTITY_HPP
#define WORLD_ENTITY_HPP
#include <ofVec2f.h>

#include <memory>

#include "../shader/Transform.hpp"
namespace planet {

class World;
class Entity : public std::enable_shared_from_this<Entity> {
       public:
		static const std::string TAG;
        explicit Entity(std::weak_ptr<World> worldRef);
        virtual ~Entity() = default;

        virtual void update();
        virtual void draw();

        void destroy();
        bool isDestroyed() const;

        Transform& getTransform();
        const Transform& getTransform() const;
        std::shared_ptr<World> getWorld() const;
		virtual std::string getTag() const;

       protected:
        virtual void onDestroy();
        Transform transform;

       private:
        bool destroyed;
        std::weak_ptr<World> worldRef;
};
}  // namespace planet
#endif