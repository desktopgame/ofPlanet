#pragma once
#ifndef WORLD_WORLD_HPP
#define WORLD_WORLD_HPP
#include <ofCamera.h>
#include <ofFbo.h>
#include <ofVec3f.h>

#include <array>
#include <memory>

#include "../shader/NameSet.hpp"
#include "BlockColliderType.hpp"
#include "BlockRenderer.hpp"
#include "BlockTable.hpp"
namespace planet {

class Entity;
class BlockBehavior;
class World : public std::enable_shared_from_this<World> {
       public:
        static std::shared_ptr<World> create(const NameSet& nameSet,
                                             const glm::ivec3& size);
        static std::shared_ptr<World> create(const NameSet& nameSet, int xSize,
                                             int ySize, int zSize);
        static int floatToInt(float f);

        void load(const BlockTable& table);
        void clear();

        void update();
        void drawToBuffer();
        void render();

        void invalidate();
        void rehash();

        void setBlockBehavior(glm::vec3 pos,
                              std::shared_ptr<BlockBehavior> block);
        void setBlockBehavior(glm::ivec3 pos,
                              std::shared_ptr<BlockBehavior> block);
        void setBlockBehavior(float x, float y, float z,
                              std::shared_ptr<BlockBehavior> block);
        void setBlockBehavior(int x, int y, int z,
                              std::shared_ptr<BlockBehavior> block);
        std::shared_ptr<BlockBehavior> getBlockBehavior(int x, int y,
                                                        int z) const;
        std::shared_ptr<BlockBehavior> getBlockBehavior(float x, float y,
                                                        float z) const;
        std::shared_ptr<BlockBehavior> getBlockBehavior(glm::vec3 pos) const;
        std::shared_ptr<BlockBehavior> getBlockBehavior(glm::ivec3 pos) const;

        bool raycast(glm::vec3 from, glm::vec3 to, glm::vec3& hitPos);
        glm::vec3 fixedPos(glm::vec3 from, glm::vec3 to, bool& hitBlockX,
                           bool& hitBlockY, bool& hitBlockZ);
        bool isContains(int x, int y, int z) const;
		bool isContains(const glm::ivec3& v) const;
		bool isContains(const glm::vec3& v) const;
        bool isEmpty(int x, int y, int z) const;
        int getGroundY(int x, int z) const;

        int getXSize() const;
        int getYSize() const;
        int getZSize() const;
        glm::ivec3 getSize() const;

        void setPlayMode(bool playMode);
        bool isPlayMode() const;

       private:
        static NameSet spriteNameSet(const NameSet& nameSet);
        BlockColliderType getColliderType(int x, int y, int z);
        explicit World(const NameSet& nameSet, const glm::ivec3& size);
        explicit World(const NameSet& nameSet, int xSize, int ySize, int zSize);
        std::vector<std::vector<std::vector<std::shared_ptr<BlockBehavior> > > >
            blocks;
        bool isInvalid;
        int xSize, ySize, zSize;
        BlockRenderer renderer;
        ofFbo fbo;
        bool bIsPlayMode;
};

}  // namespace planet
#endif