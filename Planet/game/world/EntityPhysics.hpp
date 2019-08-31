#ifndef GAME_WORLD_ENTITYPHYSICS_HPP
#define GAME_WORLD_ENTITYPHYSICS_HPP
#include "Entity.hpp"
class EntityPhysics : public Entity {
       public:
        explicit EntityPhysics();
        virtual ~EntityPhysics() = default;
        virtual void update(World& world) override;
        bool isGround() const;
        glm::vec3 velocity;

       protected:
        glm::vec3 getHitBlock(World& world, glm::vec3 npos, bool* outHit);
        void applyPhysics(World& world);
        bool _isGround;
};
#endif