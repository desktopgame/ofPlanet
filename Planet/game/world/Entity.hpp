#ifndef GAME_WORLD_ENTITY_HPP
#define GAME_WORLD_ENTITY_HPP
#include "../../gel/gel.hpp"
class World;
class Entity {
       public:
        explicit Entity();
        virtual ~Entity() = default;
        virtual void update(World& world);
        virtual void draw();
        void destroy();
        bool isDestroyed() const;
        gel::Transform transform;

       private:
        bool _destroy;
};
#endif