#ifndef GAME_WORLD_PLANET_HPP
#define GAME_WORLD_PLANET_HPP
#include "../input/KeyMove.hpp"
#include "../input/MouseScroll.hpp"
#include "EntityPhysics.hpp"
#include "World.hpp"
#include "biome/Biome.hpp"

class Planet {
       public:
        explicit Planet(const std::shared_ptr< gel::Shader>& shader);
        void update();
        void draw();

        void pause(bool b);
        void pause();
        void resume();
        bool isPause() const;

        void generate();
        void respawn();

        const std::shared_ptr<gel::Camera> getCamera() const;
        World& getWorld();
        std::shared_ptr<EntityPhysics> getPlayer() const;

       private:
        static float EYE_HEIGHT;
        static float MOVE_SPEED;
        static float JUMP_SPEED;
        static float ROTATE_SPEED;
        std::shared_ptr< gel::Shader> shader;
        gel::AABB worldAABB;
        gel::Random random;
        World world;
        std::shared_ptr<EntityPhysics> playerEntity;
        std::vector<std::unique_ptr<Biome> > biomeVec;
        std::shared_ptr<gel::Camera> playerCamera;
        float diffuse[4];
        float specular[4];
        float ambient[4];

        bool _isPause;
        MouseScroll mouseScroll;
        KeyMove keyMove;

        gel::MouseTrigger mouseTrigger;
        gel::Line beamLine;
        bool beamDraw;
        float beamTime;
};
#endif