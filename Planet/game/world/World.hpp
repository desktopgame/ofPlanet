#ifndef GAME_WORLD_WORLD_HPP
#define GAME_WORLD_WORLD_HPP
#include <algorithm>
#include <array>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "../../gel/gel.hpp"
#include "Block.hpp"
#include "BlockRegistry.hpp"
#include "Entity.hpp"
#include "EntityPhysics.hpp"
#include "MultiBlock.hpp"
#include "Space.hpp"
#define POSITION_SIZE (((128 * 64 * 128) * 6) * 4)
class World {
       public:
        explicit World(const std::shared_ptr<gel::Shader>& shader, int xSize,
                       int ySize, int zSize);
        ~World();
        void save(const std::string& filename);
        void load(const std::string& filename);
        void update();
        void draw();
        void clear();
        glm::vec3 raycast(glm::vec3 pos, glm::vec3 dir) const;
        std::shared_ptr<Entity> spawn(const std::shared_ptr<Entity>& entity);
        bool contains(const glm::vec3& pos) const;
        void clearBatch();
        void expand(int baseX, int baseY, int baseZ, const MultiBlock& mb);
        void setBlockAt(const glm::vec3& pos,
                        const std::shared_ptr<Block>& block);
        std::shared_ptr<Block> getBlockAt(const glm::vec3& pos) const;
        void setBlockAt(int x, int y, int z,
                        const std::shared_ptr<Block>& block);
        std::shared_ptr<Block> getBlockAt(int x, int y, int z) const;
        Space getSpace(int x, int y, int z) const;
        int getGroundY(int x, int z) const;
        int getVisibleBlockCount() const;
        int getVisibleFaceCount() const;
        int getXSize() const;
        int getYSize() const;
        int getZSize() const;

        void setPlayer(const std::weak_ptr<EntityPhysics>& player);
        std::weak_ptr<EntityPhysics> getPlayer() const;

        void setCamera(const std::weak_ptr<gel::Camera>& camera);
        std::weak_ptr<gel::Camera> getCamera() const;

       private:
        void init();
        void spawnBufferedEntities();
        void removeDestroyedEntities();
        static std::vector<std::string> split(const std::string& str);
        int _XSize;
        int _YSize;
        int _ZSize;
        int drawBlocks;
        int drawFaces;
        bool batched;
        GLuint positionsID;
        gel::PlaneBatch batch;
        std::weak_ptr<EntityPhysics> playerRef;
        std::weak_ptr<gel::Camera> cameraRef;
        std::vector<std::shared_ptr<Entity> > bufEntities;
        std::vector<std::shared_ptr<Entity> > entities;
        std::vector<std::vector<std::vector<std::shared_ptr<Block> > > > table;
};
#endif