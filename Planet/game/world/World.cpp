#include "World.hpp"

World::World(const std::shared_ptr<gel::Shader>& shader, int xSize, int ySize,
             int zSize)
    : table(),
      _XSize(xSize),
      _YSize(ySize),
      _ZSize(zSize),
      bufEntities(),
      entities(),
      positionsID(),
      batch(shader),
      batched(false),
      drawBlocks(0) {
        for (int i = 0; i < _YSize; i++) {
                auto yv = std::vector<std::vector<std::shared_ptr<Block> > >();
                for (int j = 0; j < _XSize; j++) {
                        auto xv = std::vector<std::shared_ptr<Block> >();
                        for (int k = 0; k < _ZSize; k++) {
                                xv.push_back(nullptr);
                        }
                        yv.push_back(xv);
                }
                table.push_back(yv);
        }
        batch.init(0.5f);
}

World::~World() { batch.destroy(); }

void World::save(const std::string& filename) {
        std::ofstream ofs(filename);
        ofs << "size"
            << " " << _XSize << " " << _YSize << " " << _ZSize << std::endl;
        for (int i = 0; i < _YSize; i++) {
                for (int j = 0; j < _XSize; j++) {
                        for (int k = 0; k < _ZSize; k++) {
                                if (table[i][j][k])
                                        ofs << "block"
                                            << " " << j << " " << i << " " << k
                                            << " " << table[i][j][k]->getID()
                                            << std::endl;
                                else
                                        ofs << "block"
                                            << " " << j << " " << i << " " << k
                                            << " "
                                            << "NULL" << std::endl;
                        }
                }
        }
        ofs.close();
}

void World::load(const std::string& filename) {
        std::ifstream ifs(filename);
        std::string line;
        // read size
        std::getline(ifs, line);
        auto words = split(line);
        if (words[0] != "size") {
                throw std::logic_error(
                    "invalid format: not found size specification");
        }
        auto xsize = std::stoi(words[1]);
        auto ysize = std::stoi(words[2]);
        auto zsize = std::stoi(words[3]);
        if (_XSize != xsize || _YSize != ysize || _ZSize != zsize) {
                throw std::logic_error("invalid format: not match size");
        }
        // read blocks
        while (std::getline(ifs, line)) {
                std::getline(ifs, line);
                words = split(line);
                // skip commented line
                if (line[0] == '#') {
                        continue;
                }
                if (words[0] != "block") {
                        throw std::logic_error("invalid format: " + line);
                }
                auto x = std::stoi(words[1]);
                auto y = std::stoi(words[2]);
                auto z = std::stoi(words[3]);
                auto id = words[4];
                if (id == "NULL") {
                        table[y][x][z] = nullptr;
                } else {
                        table[y][x][z] = BlockRegistry::getInstance().getBlock(
                            std::stoi(id));
                }
        }
        ifs.close();
}

void World::update() {
        spawnBufferedEntities();
        /*
for (int i = 0; i < _YSize; i++) {
        for (int j = 0; j < _XSize; j++) {
                for (int k = 0; k < _ZSize; k++) {
                        if (table[i][j][k])
                                table[i][j][k]->update();
                }
        }
}
        */
        for (auto e : entities) {
                e->update(*this);
        }
        removeDestroyedEntities();
}

void World::draw() {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // glTranslatef(-(_XSize / 2), 0, -(_ZSize / 2));
        drawFaces = 0;
        Space empty;
        if (!batched) {
                batch.clear();
                init();
                this->batched = true;
        }
        batch.render();
        for (auto e : entities) {
                e->draw();
        }
}

void World::clear() {
        for (int i = 0; i < _YSize; i++) {
                for (int j = 0; j < _XSize; j++) {
                        for (int k = 0; k < _ZSize; k++) {
                                table[i][j][k] = nullptr;
                        }
                }
        }
        entities.clear();
}

glm::vec3 World::raycast(glm::vec3 pos, glm::vec3 dir) const {
        std::shared_ptr<Block> ret = nullptr;
        glm::vec3 vpos = pos;
        while (!ret) {
                ret = table[Block::floatToInt(pos.y)][Block::floatToInt(pos.x)]
                           [Block::floatToInt(pos.z)];
                pos += dir;
                vpos = pos;
                if (!contains(pos)) {
                        break;
                }
        }
        return vpos;
}

std::shared_ptr<Entity> World::spawn(const std::shared_ptr<Entity>& entity) {
        bufEntities.push_back(entity);
        return entity;
}

bool World::contains(const glm::vec3& pos) const {
        auto x = Block::floatToInt(pos.x);
        auto y = Block::floatToInt(pos.y);
        auto z = Block::floatToInt(pos.z);
        return x >= 0 && x < _XSize && y >= 0 && y < _YSize && z >= 0 &&
               z < _ZSize;
}

void World::clearBatch() { this->batched = false; }

void World::expand(int baseX, int baseY, int baseZ, const MultiBlock& mb) {
        BlockRegistry& br = BlockRegistry::getInstance();
        auto data = mb.to3DData();
        for (auto e : data) {
                int x = baseX + (int)e.first.x;
                int y = baseY + (int)e.first.y;
                int z = baseZ + (int)e.first.z;
                int id = mb.decode(e.second);
                if (x >= _XSize || y >= _YSize || z >= _ZSize) {
                        continue;
                }
                if (id < 0) {
                        setBlockAt(x, y, z, nullptr);
                } else {
                        setBlockAt(x, y, z, br.getBlock(id));
                }
        }
}

void World::setBlockAt(const glm::vec3& pos,
                       const std::shared_ptr<Block>& block) {
        setBlockAt(Block::floatToInt(pos.x), Block::floatToInt(pos.y),
                   Block::floatToInt(pos.z), block);
}

std::shared_ptr<Block> World::getBlockAt(const glm::vec3& pos) const {
        return getBlockAt(Block::floatToInt(pos.x), Block::floatToInt(pos.y),
                          Block::floatToInt(pos.z));
}

void World::setBlockAt(int x, int y, int z,
                       const std::shared_ptr<Block>& block) {
        table[y][x][z] = block;
}

std::shared_ptr<Block> World::getBlockAt(int x, int y, int z) const {
        return table[y][x][z];
}

Space World::getSpace(int x, int y, int z) const {
        Space space;
        if (y < _YSize - 1) {
                space.posY = getBlockAt(x, y + 1, z);
        }
        if (y > 0) {
                space.negY = getBlockAt(x, y - 1, z);
        }
        if (x < _XSize - 1) {
                space.posX = getBlockAt(x + 1, y, z);
        }
        if (x > 0) {
                space.negX = getBlockAt(x - 1, y, z);
        }
        if (z < _ZSize - 1) {
                space.posZ = getBlockAt(x, y, z + 1);
        }
        if (z > 0) {
                space.negZ = getBlockAt(x, y, z - 1);
        }
        return space;
}

int World::getGroundY(int x, int z) const {
        for (int i = 1; i < getYSize(); i++) {
                auto blk = getBlockAt(x, getYSize() - i, z);
                if (blk != nullptr) {
                        return getYSize() - i + 1;
                }
        }
        return 1;
}

int World::getVisibleBlockCount() const { return _YSize * _XSize * _ZSize; }

int World::getVisibleFaceCount() const { return drawFaces; }

int World::getXSize() const { return _XSize; }
int World::getYSize() const { return _YSize; }
int World::getZSize() const { return _ZSize; }

void World::setPlayer(const std::weak_ptr<EntityPhysics>& player) {
        this->playerRef = player;
}

std::weak_ptr<EntityPhysics> World::getPlayer() const { return playerRef; }

void World::setCamera(const std::weak_ptr<gel::Camera>& camera) {
        this->cameraRef = camera;
}

std::weak_ptr<gel::Camera> World::getCamera() const { return cameraRef; }

void World::init() {
        BlockRegistry& br = BlockRegistry::getInstance();
        // set position
        Space space;
        int posIndex = 0;
        batch.clear();
        for (int i = 0; i < _XSize; i++) {
                for (int j = 0; j < _YSize; j++) {
                        for (int k = 0; k < _ZSize; k++) {
                                int x = i;
                                int y = j;
                                int z = k;
                                if (table[y][x][z]) {
                                        table[y][x][z]->batch(
                                            batch, x, y, z, getSpace(x, y, z));
                                        drawBlocks++;
                                }
                        }
                }
        }
}

void World::spawnBufferedEntities() {
        for (int i = 0; i < bufEntities.size(); i++) {
                entities.push_back(bufEntities[i]);
        }
        bufEntities.clear();
}

void World::removeDestroyedEntities() {
        auto iter = entities.begin();
        while (iter != entities.end()) {
                auto v = *iter;
                if (v->isDestroyed()) {
                        iter = entities.erase(iter);
                } else {
                        ++iter;
                }
        }
}

std::vector<std::string> World::split(const std::string& str) {
        std::vector<std::string> words;
        std::string buf;
        for (auto c : str) {
                if (c != ' ') {
                        buf += c;
                } else {
                        words.push_back(buf);
                        buf.clear();
                }
        }
        if (!buf.empty()) {
                words.push_back(buf);
        }
        return words;
}