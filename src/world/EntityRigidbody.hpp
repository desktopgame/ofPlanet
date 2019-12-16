#pragma once
#ifndef WORLD_ENTITYRIGIDBODY_HPP
#define WORLD_ENTITYRIGIDBODY_HPP
#include "Entity.hpp"
namespace planet {

class EntityRigidbody : public Entity {
       public:
        explicit EntityRigidbody(std::weak_ptr<World> worldRef);
        virtual void update() override;
        glm::vec3 velocity;

       protected:
        void startJump();
        void computePhysics();

        float jumpVelocity;
        bool jumpTriggered;
        bool bIsGround;

       private:
};
}  // namespace planet
#endif