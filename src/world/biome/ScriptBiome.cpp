#include "ScriptBiome.hpp"

#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>


#include "../Block.hpp"
#include "../BlockPack.hpp"
#include "../MultiBlock.hpp"
#include "../csvr/Parser.hpp"

using namespace ofxLua;
namespace planet {
ScriptBiome::ScriptBiome(const std::string& filename)
    : lua(), ctx(nullptr), table(nullptr), multiBlockMap(), weightTableMap() {
        lua.define("setblock", lua_setblock);
        lua.define("putblock", lua_putblock);
        lua.define("getblock", lua_getblock);
        lua.define("setblockrange", lua_setblockrange);
        lua.define("putblockrange", lua_putblockrange);
        lua.define("replaceblockrange", lua_replaceblockrange);
        lua.define("getxsize", lua_getxsize);
        lua.define("getysize", lua_getysize);
        lua.define("getzsize", lua_getzsize);
        lua.define("newstruct", lua_newstruct);
        lua.define("genstruct", lua_genstruct);
        lua.define("expandstruct", lua_expandstruct);
        lua.define("setweight", lua_setweight);
        lua.define("getweight", lua_getweight);
        lua.define("setweightrange", lua_setweightrange);
        lua.loadFile(filename);
        this->globals = lua.getAllVariables().get();
}

void ScriptBiome::onGUI() {
        // GUIを表示する
        auto iter = globals.begin();
        std::unordered_map<std::string, Object> copy;
        while (iter != globals.end()) {
                auto kv = *iter;
                auto key = kv.first;
                auto obj = kv.second;
                if (obj.type == T_NUMBER) {
                        // luaには実数型と整数型の区別がないので頭文字で判別する
                        if (key[0] == 'i') {
                                int v = static_cast<int>(obj.value.number);
                                if (ImGui::SliderInt(key.c_str(), &v, -1000,
                                                     1000)) {
                                        lua.setGlobalInt(key, v);
                                }
                                obj.value.number = static_cast<double>(v);
                        } else {
                                float v = static_cast<float>(obj.value.number);
                                if (ImGui::SliderFloat(key.c_str(), &v, -1000,
                                                       1000)) {
                                        lua.setGlobalNumber(
                                            key, static_cast<double>(v));
                                }
                                obj.value.number = static_cast<double>(v);
                        }
                } else if (obj.type == T_BOOL) {
                        if (ImGui::Checkbox(key.c_str(), &obj.value.boolean)) {
                                lua.setGlobalBool(key, obj.value.boolean);
                        }
                } else if (obj.type == T_STRING) {
                        char buf[256];
                        memset(buf, '\0', 256);
                        memcpy(buf, obj.value.string.c_str(), 256);
                        // FIXME:256文字以上のテキストが入った場合
                        if (ImGui::InputText(key.c_str(), buf, 256)) {
                                lua.setGlobalString(key, buf);
                        }
                        obj.value.string = buf;
                }
                copy.insert_or_assign(key, obj);
                ++iter;
        }
        this->globals = copy;
}

bool ScriptBiome::isUseCallbacks() { return this->mode == "default"; }

void ScriptBiome::onBeginGenerateWorld(BlockTable& blockTable) {
        this->ctx = Context::push();
        this->table = std::make_shared<BlockTable>(blockTable.getXSize(),
                                                   blockTable.getYSize(),
                                                   blockTable.getZSize());
        this->multiBlockMap = std::make_shared<MultiBlockMap>();
        this->weightTableMap =
            std::make_shared<std::unordered_map<std::string, WeightTable> >();
        ctx->set("TABLE", table);
        ctx->set("MB", multiBlockMap);
        ctx->set("HM", heightMap);
        ctx->set("WT", weightTableMap);
        // コールバックモードを決める
        std::vector<Variant> modeV = lua.call("start", std::vector<Object>{},
                                              std::vector<Type>{T_STRING})
                                         .get();
        this->mode = modeV.at(0).string;
        if (this->mode != "default" && this->mode != "ignore") {
                std::cerr << "`" << mode << "` mode is undefined." << std::endl;
        }
}

void ScriptBiome::onEndGenerateWorld(BlockTable& blockTable) {
        lua.call("onPostGenerate", std::vector<Object>{}, std::vector<Type>{})
            .check();
        //コンテキストを削除
        Context::pop();
        this->ctx = nullptr;
        // 出力をコピー
        for (int x = 0; x < this->table->getXSize(); x++) {
                for (int y = 0; y < this->table->getYSize(); y++) {
                        for (int z = 0; z < this->table->getZSize(); z++) {
                                blockTable.set(x, y, z,
                                               this->table->get(x, y, z));
                        }
                }
        }
}

void ScriptBiome::onEndGenerateTerrain() { ctx->set("HM", heightMap); }

float ScriptBiome::onFixHeight(float y) {
        std::vector<Variant> r =
            lua.call("onFixHeight", std::vector<Object>{create(y)},
                     std::vector<Type>{T_NUMBER})
                .get();
        return static_cast<float>(r.at(0).number);
}

void ScriptBiome::onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                    int z) {
        std::vector<Variant> r =
            lua.call("onGenerateTerrain",
                     std::vector<Object>{create(x), create(y), create(z)},
                     std::vector<Type>{})
                .get();
}

void ScriptBiome::onGenerateStructures(BlockTable& blockTable) {
        std::vector<Variant> r =
            lua.call("onGenerateStructures", std::vector<Object>{},
                     std::vector<Type>{})
                .get();
}

void ScriptBiome::onGenerateCave(BlockTable& blockTable, int x, int y, int z,
                                 float noise) {
        std::vector<Variant> r =
            lua.call("onGenerateCave",
                     std::vector<Object>{create(x), create(y), create(z),
                                         create(static_cast<double>(noise))},
                     std::vector<Type>{})
                .get();
}

int lua_setblock(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        int x = luaL_checkinteger(state, -4);
        int y = luaL_checkinteger(state, -3);
        int z = luaL_checkinteger(state, -2);
        x = std::max(0, std::min(x, table->getXSize() - 1));
        y = std::max(0, std::min(y, table->getYSize() - 1));
        z = std::max(0, std::min(z, table->getZSize() - 1));
        std::string name = luaL_checkstring(state, -1);
        int id = BlockPack::getCurrent()->getBlockIndexForName(name);
        table->set(x, y, z, BlockPrefab(id, false));
        return 0;
}

int lua_putblock(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        int x = luaL_checkinteger(state, -4);
        int y = luaL_checkinteger(state, -3);
        int z = luaL_checkinteger(state, -2);
        x = std::max(0, std::min(x, table->getXSize() - 1));
        y = std::max(0, std::min(y, table->getYSize() - 1));
        z = std::max(0, std::min(z, table->getZSize() - 1));
        std::string name = luaL_checkstring(state, -1);
        int id = BlockPack::getCurrent()->getBlockIndexForName(name);
        if (table->get(x, y, z).id == -1) {
                table->set(x, y, z, BlockPrefab(id, false));
        }
        return 0;
}

int lua_getblock(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        int x = luaL_checkinteger(state, -3);
        int y = luaL_checkinteger(state, -2);
        int z = luaL_checkinteger(state, -1);
        x = std::max(0, std::min(x, table->getXSize() - 1));
        y = std::max(0, std::min(y, table->getYSize() - 1));
        z = std::max(0, std::min(z, table->getZSize() - 1));
        lua_pushstring(
            state,
            blockpack->getBlock(table->get(x, y, z).id)->getName().c_str());
        return 1;
}

int lua_setblockrange(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        int minX = luaL_checkinteger(state, -7);
        int minY = luaL_checkinteger(state, -6);
        int minZ = luaL_checkinteger(state, -5);
        int maxX = luaL_checkinteger(state, -4);
        int maxY = luaL_checkinteger(state, -3);
        int maxZ = luaL_checkinteger(state, -2);
        std::string name = luaL_checkstring(state, -1);
        int id = BlockPack::getCurrent()->getBlockIndexForName(name);
        for (int x = minX; x <= maxX; x++) {
                for (int y = minY; y <= maxY; y++) {
                        for (int z = minZ; z <= maxZ; z++) {
                                table->set(x, y, z, BlockPrefab(id, false));
                        }
                }
        }
        return 0;
}

int lua_putblockrange(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        int minX = luaL_checkinteger(state, -7);
        int minY = luaL_checkinteger(state, -6);
        int minZ = luaL_checkinteger(state, -5);
        int maxX = luaL_checkinteger(state, -4);
        int maxY = luaL_checkinteger(state, -3);
        int maxZ = luaL_checkinteger(state, -2);
        std::string name = luaL_checkstring(state, -1);
        int id = BlockPack::getCurrent()->getBlockIndexForName(name);
        for (int x = minX; x <= maxX; x++) {
                for (int y = minY; y <= maxY; y++) {
                        for (int z = minZ; z <= maxZ; z++) {
                                if (table->get(x, y, z).id == -1) {
                                        table->set(x, y, z,
                                                   BlockPrefab(id, false));
                                }
                        }
                }
        }
        return 0;
}

int lua_replaceblockrange(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        int minX = luaL_checkinteger(state, -8);
        int minY = luaL_checkinteger(state, -7);
        int minZ = luaL_checkinteger(state, -6);
        int maxX = luaL_checkinteger(state, -5);
        int maxY = luaL_checkinteger(state, -4);
        int maxZ = luaL_checkinteger(state, -3);
        std::string oldName = luaL_checkstring(state, -2);
        std::string newName = luaL_checkstring(state, -1);
        int oldId = BlockPack::getCurrent()->getBlockIndexForName(oldName);
        int newId = BlockPack::getCurrent()->getBlockIndexForName(newName);
        for (int x = minX; x <= maxX; x++) {
                for (int y = minY; y <= maxY; y++) {
                        for (int z = minZ; z <= maxZ; z++) {
                                if (table->get(x, y, z).id == oldId) {
                                        table->set(x, y, z,
                                                   BlockPrefab(newId, false));
                                }
                        }
                }
        }
        return 0;
}

int lua_getxsize(lua_State* state) {
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        lua_pushinteger(state, table->getXSize());
        return 1;
}

int lua_getysize(lua_State* state) {
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        lua_pushinteger(state, table->getYSize());
        return 1;
}

int lua_getzsize(lua_State* state) {
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        lua_pushinteger(state, table->getZSize());
        return 1;
}

int lua_newstruct(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        std::string name = luaL_checkstring(state, -2);
        std::string body = luaL_checkstring(state, -1);
        auto pack = BlockPack::getCurrent();
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        auto mbmap = Context::top()->get<std::shared_ptr<MultiBlockMap> >("MB");
        MultiBlock mb;
        // CSVR形式を解析
        csvr::Parser parser;
        parser.parse(body);
        for (int i = 0; i < parser.getTableCount(); i++) {
                auto& table = parser.getTableAt(i);
                MultiBlockLayer mbLayer;
                for (int j = 0; j < static_cast<int>(table.size()); j++) {
                        auto& line = table.at(j);
                        MultiBlockLine mbLine;
                        for (int k = 0; k < static_cast<int>(line.size());
                             k++) {
                                auto& col = line.at(k);
                                mbLine.emplace_back(col);
                        }
                        mbLayer.emplace_back(mbLine);
                }
                mb.emplace_back(mbLayer);
        }
        mbmap->insert_or_assign(name, mb);
        return 0;
}

int lua_genstruct(lua_State* state) {
        int maxWeight = luaL_checkinteger(state, -3);
        int limitWeight = luaL_checkinteger(state, -2);
        std::string name = luaL_checkstring(state, -1);

        using HeightMapT =
            std::unordered_map<glm::ivec2, int, hidden::Vec2HashFunc,
                               hidden::Vec2HashFunc>;
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        auto mbmap = Context::top()->get<std::shared_ptr<MultiBlockMap> >("MB");
        auto hmap = Context::top()->get<HeightMapT>("HM");
        auto wtableMap =
            Context::top()
                ->get<std::shared_ptr<
                    std::unordered_map<std::string, WeightTable> > >("WT");
        //構造物ごとに重み付けを記録する
        if (!wtableMap->count(name)) {
                WeightTable wt(table->getXSize(), table->getYSize(),
                               table->getZSize());
                wtableMap->insert_or_assign(name, wt);
        }
        auto& wtable = wtableMap->at(name);

        auto& mb = mbmap->at(name);
        // 構造物のだいたいの大きさを取得する
        glm::ivec3 mbSize;
        multiBlock3DSize(mb, mbSize);
        // 全てのエリアに対して
        auto blockAreaVec = table->getAllBlockAreaForTop();
        for (auto& blockArea : blockAreaVec) {
                auto areaSize = blockArea.compute2DSize();
                // 高さが足りないので次へ
                int stackHeight = table->getStackableHeight(blockArea);
                if (stackHeight < mbSize.y) {
                        continue;
                }
                areaSize.y = stackHeight;
                // 幅がたりないので次へ
                if (areaSize.x < mbSize.x || areaSize.z < mbSize.z) {
                        continue;
                }
                std::vector<glm::ivec3> expandPosVec;
                // 一ますごとに配置可能か検証する
                for (int i = 0; i < blockArea.getPointCount(); i++) {
                        auto point = blockArea.getPoint(i);
                        point += glm::ivec3(0, 1, 0);
                        bool canPlace =
                            table->canExpand(point.x, point.y, point.z, mb);
                        if (canPlace) {
                                expandPosVec.emplace_back(point);
                        }
                }
                // シャッフルしてから適当につっこむ
                std::random_device seed_gen;
                std::mt19937 engine(seed_gen());
                std::shuffle(expandPosVec.begin(), expandPosVec.end(), engine);
                for (auto point : expandPosVec) {
                        auto expandVec =
                            table->expandTargets(point.x, point.y, point.z, mb);
                        bool canPlace = true;
                        // 重み付けによって判定する
                        for (auto& expandBlock : expandVec) {
                                glm::ivec3 expandPos = std::get<0>(expandBlock);
                                int weight = wtable.getWeight(
                                    expandPos.x, expandPos.y, expandPos.z);
                                if (limitWeight <= weight) {
                                        canPlace = false;
                                        break;
                                }
                        }
                        if (!canPlace) {
                                continue;
                        }
                        // 展開する
                        table->expand(point.x, point.y, point.z, mb);
                        // 中心の位置を取得
                        auto expandCenter = point;
                        expandCenter.x += mbSize.x / 2;
                        expandCenter.y += mbSize.y / 2;
                        expandCenter.z += mbSize.z / 2;
                        // 中心から重み付けを加算する
                        wtable.addWeight(expandCenter.x, expandCenter.y,
                                         expandCenter.z, maxWeight);
                }
        }
        return 0;
}

int lua_expandstruct(lua_State* state) {
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        auto mbmap = Context::top()->get<std::shared_ptr<MultiBlockMap> >("MB");
        int x = luaL_checkinteger(state, -4);
        int y = luaL_checkinteger(state, -3);
        int z = luaL_checkinteger(state, -2);
        x = std::max(0, std::min(x, table->getXSize() - 1));
        y = std::max(0, std::min(y, table->getYSize() - 1));
        z = std::max(0, std::min(z, table->getZSize() - 1));
        std::string name = luaL_checkstring(state, -1);
        auto& mb = mbmap->at(name);
        table->expand(x, y, z, mb);
        return 0;
}
int lua_setweight(lua_State* state) {
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        auto mbmap = Context::top()->get<std::shared_ptr<MultiBlockMap> >("MB");
        auto wtableMap =
            Context::top()
                ->get<std::shared_ptr<
                    std::unordered_map<std::string, WeightTable> > >("WT");
        int x = luaL_checkinteger(state, -5);
        int y = luaL_checkinteger(state, -4);
        int z = luaL_checkinteger(state, -3);
        int weight = luaL_checkinteger(state, -2);
        x = std::max(0, std::min(x, table->getXSize() - 1));
        y = std::max(0, std::min(y, table->getYSize() - 1));
        z = std::max(0, std::min(z, table->getZSize() - 1));
        std::string name = luaL_checkstring(state, -1);
        //構造物ごとに重み付けを記録する
        if (!wtableMap->count(name)) {
                WeightTable wt(table->getXSize(), table->getYSize(),
                               table->getZSize());
                wtableMap->insert_or_assign(name, wt);
        }
        auto& wtable = wtableMap->at(name);
        wtable.setWeight(x, y, z, weight);
        return 0;
}
int lua_getweight(lua_State* state) {
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        auto mbmap = Context::top()->get<std::shared_ptr<MultiBlockMap> >("MB");
        auto wtableMap =
            Context::top()
                ->get<std::shared_ptr<
                    std::unordered_map<std::string, WeightTable> > >("WT");
        int x = luaL_checkinteger(state, -4);
        int y = luaL_checkinteger(state, -3);
        int z = luaL_checkinteger(state, -2);
        x = std::max(0, std::min(x, table->getXSize() - 1));
        y = std::max(0, std::min(y, table->getYSize() - 1));
        z = std::max(0, std::min(z, table->getZSize() - 1));
        std::string name = luaL_checkstring(state, -1);
        //構造物ごとに重み付けを記録する
        if (!wtableMap->count(name)) {
                WeightTable wt(table->getXSize(), table->getYSize(),
                               table->getZSize());
                wtableMap->insert_or_assign(name, wt);
        }
        auto& wtable = wtableMap->at(name);
        lua_pushinteger(state, wtable.getWeight(x, y, z));
        return 1;
}
int lua_setweightrange(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        auto wtableMap =
            Context::top()
                ->get<std::shared_ptr<
                    std::unordered_map<std::string, WeightTable> > >("WT");
        int minX = luaL_checkinteger(state, -8);
        int minY = luaL_checkinteger(state, -7);
        int minZ = luaL_checkinteger(state, -6);
        int maxX = luaL_checkinteger(state, -5);
        int maxY = luaL_checkinteger(state, -4);
        int maxZ = luaL_checkinteger(state, -3);
        int weight = luaL_checkinteger(state, -2);
        std::string name = luaL_checkstring(state, -1);
        //構造物ごとに重み付けを記録する
        if (!wtableMap->count(name)) {
                WeightTable wt(table->getXSize(), table->getYSize(),
                               table->getZSize());
                wtableMap->insert_or_assign(name, wt);
        }
        auto& wtable = wtableMap->at(name);
        for (int x = minX; x <= maxX; x++) {
                for (int y = minY; y <= maxY; y++) {
                        for (int z = minZ; z <= maxZ; z++) {
                                wtable.setWeight(x, y, z, weight);
                        }
                }
        }
        return 0;
}
}  // namespace planet