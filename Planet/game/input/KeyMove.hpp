#ifndef GAME_INPUT_KEYMOVE_HPP
#define GAME_INPUT_KEYMOVE_HPP
#include "../../gel/gel.hpp"
class KeyMove {
       public:
        explicit KeyMove(const gel::Transform& transform, float moveSpeed,
                         float jumpSpeed);
        void update(bool isGround);
        glm::vec3 getVelocity() const;
        bool isJumping() const;

       private:
        float moveSpeed;
        float jumpSpeed;
        const gel::Transform& transform;
        glm::vec3 force;
        bool _isJumping;
};
#endif