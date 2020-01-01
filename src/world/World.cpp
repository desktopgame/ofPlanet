#include "World.hpp"

#include <ofAppRunner.h>
#include <ofEasyCam.h>
#include <ofFbo.h>
#include <ofLight.h>
#include <ofVec3f.h>
#include <ofxAssimpModelLoader.h>

#include <string>

#include "../common/Stopwatch.hpp"
#include "../game/ofApp.h"
#include "../shader/Camera.hpp"
#include "../shader/CameraRegistry.hpp"
#include "Block.hpp"
#include "BlockPack.hpp"
#include "BlockRenderer.hpp"
#include "Camera.hpp"
#include "TexturePack.hpp"
namespace planet {

std::shared_ptr<World> World::create(const NameSet& nameSet,
                                     const glm::ivec3& size) {
        return create(nameSet, size.x, size.y, size.z);
}

std::shared_ptr<World> World::create(const NameSet& nameSet, int xSize,
                                     int ySize, int zSize) {
        World* world = new World(nameSet, xSize, ySize, zSize);
        std::shared_ptr<World> ret = std::shared_ptr<World>(world);
        for (int i = 0; i < xSize; i++) {
                std::vector<std::vector<std::shared_ptr<BlockBehavior> > >
                    yline;
                for (int j = 0; j < ySize; j++) {
                        std::vector<std::shared_ptr<BlockBehavior> > zline;
                        for (int k = 0; k < zSize; k++) {
                                zline.emplace_back(nullptr);
                        }
                        yline.emplace_back(zline);
                }
                ret->blocks.emplace_back(yline);
        }
        return ret;
}

int World::floatToInt(float f) { return static_cast<int>(std::round(f)); }

void World::load(const BlockTable& table) {
		auto bp = BlockPack::getCurrent();
        for (int x = 0; x < table.getXSize(); x++) {
                for (int y = 0; y < table.getYSize(); y++) {
                        for (int z = 0; z < table.getZSize(); z++) {
                                BlockPrefab i = table.get(x, y, z);
                                if (i.id == -1) {
                                        continue;
                                }
                                if (i.instanced) {
                                        setBlockBehavior(
                                            x, y, z,
                                            bp->getBlock(i.id)
                                                ->newBehavior());
                                } else {
                                        setBlockBehavior(
                                            x, y, z, bp->getBlock(i.id));
                                }
                        }
                }
        }
}

void World::clear() {
        for (int x = 0; x < xSize; x++) {
                for (int y = 0; y < ySize; y++) {
                        for (int z = 0; z < zSize; z++) {
                                setBlockBehavior(x, y, z, nullptr);
                        }
                }
        }
}

void World::update() {
        if (!isPlayMode()) {
                return;
        }
}

void World::drawToBuffer() {
        rehash();
        // screenBuffer.bind();
        fbo.begin();
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO,
                            GL_ONE);
        glEnable(GL_DEPTH_TEST);
        // ofClear(0);
        fbo.clear();
        ofEnableAlphaBlending();
        ofSetBackgroundColor(0, 0, 0, 0);
        renderer.render();
        fbo.end();
        // screenBuffer.unbind();
}

void World::render() {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
        // screenBuffer.render();
}

void World::invalidate() { this->isInvalid = true; }

void World::rehash() {
        if (!isInvalid) {
                return;
        }
        if (!fbo.isAllocated()) {
                fbo.allocate(ofGetWidth(), ofGetHeight());
        }
        this->isInvalid = false;
        renderer.clear();
        auto w = std::const_pointer_cast<World>(shared_from_this());
        for (int x = 0; x < xSize; x++) {
                for (int y = 0; y < ySize; y++) {
                        for (int z = 0; z < zSize; z++) {
                                auto block = getBlockBehavior(x, y, z);
                                if (!block) {
                                        continue;
                                }
                                block->batch(w, renderer, x, y, z);
                        }
                }
        }
        renderer.rehash();
}

void World::setBlockBehavior(glm::vec3 pos,
                             std::shared_ptr<BlockBehavior> block) {
        setBlockBehavior(pos.x, pos.y, pos.z, block);
}

void World::setBlockBehavior(glm::ivec3 pos,
                             std::shared_ptr<BlockBehavior> block) {
        setBlockBehavior(pos.x, pos.y, pos.z, block);
}

void World::setBlockBehavior(float x, float y, float z,
                             std::shared_ptr<BlockBehavior> block) {
        setBlockBehavior(World::floatToInt(x), World::floatToInt(y),
                         World::floatToInt(z), block);
}

void World::setBlockBehavior(int x, int y, int z,
                             std::shared_ptr<BlockBehavior> block) {
        blocks[x][y][z] = block;
        this->isInvalid = true;
}

std::shared_ptr<BlockBehavior> World::getBlockBehavior(int x, int y,
                                                       int z) const {
        return blocks[x][y][z];
}
std::shared_ptr<BlockBehavior> World::getBlockBehavior(float x, float y,
                                                       float z) const {
        return getBlockBehavior(World::floatToInt(x), World::floatToInt(y),
                                World::floatToInt(z));
}
std::shared_ptr<BlockBehavior> World::getBlockBehavior(glm::vec3 pos) const {
        return getBlockBehavior(pos.x, pos.y, pos.z);
}
std::shared_ptr<BlockBehavior> World::getBlockBehavior(glm::ivec3 pos) const {
        return getBlockBehavior(pos.x, pos.y, pos.z);
}
bool World::raycast(glm::vec3 from, glm::vec3 to, glm::vec3& hitPos) {
        int x = World::floatToInt(from.x);
        int y = World::floatToInt(from.y);
        int z = World::floatToInt(from.z);

        // auto npos = transform.position + velocity;
        int nx = World::floatToInt(to.x);
        int ny = World::floatToInt(to.y);
        int nz = World::floatToInt(to.z);

        int minX = std::min(x, nx);
        int maxX = std::max(x, nx);
        int minY = std::min(y, ny);
        int maxY = std::max(y, ny);
        int minZ = std::min(z, nz);
        int maxZ = std::max(z, nz);

        int hitX = -1;
        int hitY = -1;
        int hitZ = -1;
        bool hit = false;

        for (int i = minX; i <= maxX; i++) {
                for (int j = minY; j <= maxY; j++) {
                        for (int k = minZ; k <= maxZ; k++) {
                                auto pt = glm::vec3(i, j, k);
                                if (!isContains(i, j, k) ||
                                    getColliderType(i, j, k) ==
                                        BlockColliderType::Hit) {
                                        hit = true;
                                        hitX = i;
                                        hitY = j;
                                        hitZ = k;
                                }
                                if (hit) break;
                        }
                        if (hit) break;
                }
                if (hit) break;
        }
        hitPos.x = hitX;
        hitPos.y = hitY;
        hitPos.z = hitZ;
        return hit;
}
glm::vec3 World::fixedPos(glm::vec3 from, glm::vec3 to, bool& hitBlockX,
                          bool& hitBlockY, bool& hitBlockZ) {
        auto newPos = from;
        auto xNewPos = glm::vec3(to.x, from.y, from.z);
        bool isXHit = false;
        glm::vec3 hitX;
        if (!raycast(from, xNewPos, hitX)) {
                newPos.x = xNewPos.x;
        } else {
                if (to.x > from.x) {
                        newPos.x = hitX.x - 1;
                } else {
                        newPos.x = hitX.x + 1;
                }
                hitBlockX = true;
        }
        auto yNewPos = glm::vec3(from.x, to.y, from.z);
        bool isYHit = false;
        glm::vec3 hitY;
        if (!raycast(from, yNewPos, hitY)) {
                newPos.y = yNewPos.y;
        } else {
                if (to.y > from.y) {
                        newPos.y = hitY.y - 1;
                } else {
                        newPos.y = hitY.y + 1;
                }
                hitBlockY = true;
        }
        auto zNewPos = glm::vec3(from.x, from.y, to.z);
        bool isZHit = false;
        glm::vec3 hitZ;
        if (!raycast(from, zNewPos, hitZ)) {
                newPos.z = zNewPos.z;
        } else {
                if (to.z > from.z) {
                        newPos.z = hitZ.z - 1;
                } else {
                        newPos.z = hitZ.z + 1;
                }
                hitBlockZ = true;
        }
        return newPos;
}
bool World::isContains(int x, int y, int z) const {
        if (x < 0 || x >= xSize) return false;
        if (y < 0 || y >= ySize) return false;
        if (z < 0 || z >= zSize) return false;
        return true;
}
bool World::isContains(const glm::ivec3 & v) const {
	return isContains(v.x, v.y, v.z);
}
bool World::isContains(const glm::vec3 & v) const {
	return isContains(floatToInt(v.x), floatToInt(v.y), floatToInt(v.z));
}
bool World::isEmpty(int x, int y, int z) const {
        if (!isContains(x, y, z)) {
                return true;
        }
        auto block = getBlockBehavior(x, y, z);
        return block == nullptr;
}
int World::getGroundY(int x, int z) const {
        for (int i = 0; i < ySize; i++) {
                if (!getBlockBehavior(x, i, z)) {
                        return i;
                }
        }
        return ySize;
}
int World::getXSize() const { return xSize; }
int World::getYSize() const { return ySize; }
int World::getZSize() const { return zSize; }
glm::ivec3 World::getSize() const { return glm::ivec3(xSize, ySize, zSize); }
void World::setPlayMode(bool playMode) {
        this->bIsPlayMode = playMode;
}
bool World::isPlayMode() const { return bIsPlayMode; }

NameSet World::spriteNameSet(const NameSet& nameSet) {
        NameSet ns = nameSet;
        ns.shader = "Sprite";
        return ns;
}
BlockColliderType World::getColliderType(int x, int y, int z) {
        auto block = getBlockBehavior(x, y, z);
        if (!block) {
                return BlockColliderType::NoHit;
        }
        auto w = std::const_pointer_cast<World>(shared_from_this());
        return block->getColliderType(w, x, y, z);
}
// private
World::World(const NameSet& nameSet, const glm::ivec3& size)
    : World(nameSet, size.x, size.y, size.z) {}

World::World(const NameSet& nameSet, int xSize, int ySize, int zSize)
	: blocks(),
	xSize(xSize),
	ySize(ySize),
	zSize(zSize),
	renderer(nameSet),
	isInvalid(true),
	bIsPlayMode(false),
	fbo() {
}
}  // namespace planet
