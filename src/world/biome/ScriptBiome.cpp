#include "ScriptBiome.hpp"

#include "../Block.hpp"
#include "../BlockPack.hpp"
#include "../luaex/luaimpl.hpp"

namespace planet {
using namespace luaex;
ScriptBiome::ScriptBiome(const std::string& filename)
    : lua(), ctx(nullptr), table(nullptr) {
        lua.define("setblock", lua_setblock);
        lua.define("getblock", lua_getblock);
        lua.define("getxsize", lua_getxsize);
        lua.define("getysize", lua_getysize);
        lua.define("getzsize", lua_getzsize);
        lua.loadFile(filename);
        this->globals = lua.getAllVariables();
}

void ScriptBiome::onGUI() {
        // GUIを表示する
        auto iter = globals.begin();
        std::unordered_map<std::string, luaex::Object> copy;
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

void ScriptBiome::onBeginGenerate(BlockTable& blockTable) {
        this->ctx = Context::create();
        this->table = std::make_shared<BlockTable>(blockTable.getXSize(),
                                                   blockTable.getYSize(),
                                                   blockTable.getZSize());
        ctx->select();
        ctx->put("TABLE", table);
        // コールバックモードを決める
        std::vector<Variant> modeV;
        lua.call("getCallbackMode", std::vector<Object>{},
                 std::vector<Type>{T_STRING}, modeV);
        this->mode = modeV.at(0).string;
        if (this->mode != "default" && this->mode != "ignore") {
                std::cerr << "`" << mode << "` mode is undefined." << std::endl;
        }
}

void ScriptBiome::onEndGenerate(BlockTable& blockTable) {
        lua.call("onPostGenerate", std::vector<Object>{}, std::vector<Type>{},
                 std::vector<Variant>{});
        //コンテキストを削除
        ctx->dispose();
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

float ScriptBiome::onFixHeight(float y) {
        std::vector<Variant> r;
        lua.call("onFixHeight", std::vector<Object>{create(y)},
                 std::vector<Type>{T_NUMBER}, r);
        return static_cast<float>(r.at(0).number);
}

void ScriptBiome::onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                    int z) {
        std::vector<Variant> r;
        lua.call("onGenerateTerrain",
                 std::vector<Object>{create(x), create(y), create(z)},
                 std::vector<Type>{}, r);
}

void ScriptBiome::onGenerateWater(BlockTable& blockTable, int x, int y, int z) {
        std::vector<Variant> r;
        lua.call("onGenerateWater",
                 std::vector<Object>{create(x), create(y), create(z)},
                 std::vector<Type>{}, r);
}

void ScriptBiome::onGenerateStructures(BlockTable& blockTable) {
        std::vector<Variant> r;
        lua.call("onGenerateStructures", std::vector<Object>{},
                 std::vector<Type>{}, r);
}

void ScriptBiome::onGenerateCave(BlockTable& blockTable, int x, int y, int z,
                                 float noise) {
        std::vector<Variant> r;
        lua.call("onGenerateCave",
                 std::vector<Object>{create(x), create(y), create(z),
                                     create(static_cast<double>(noise))},
                 std::vector<Type>{}, r);
}

int lua_setblock(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = linb::any_cast<std::shared_ptr<BlockTable>>(
            Context::current()->get("TABLE"));
        int x = luaL_checkinteger(state, -4);
        int y = luaL_checkinteger(state, -3);
        int z = luaL_checkinteger(state, -2);
        std::string name = luaL_checkstring(state, -1);
        int id = BlockPack::getCurrent()->getBlockIndexForName(name);
        table->set(x, y, z, BlockPrefab(id, false));
        return 0;
}
int lua_getblock(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = linb::any_cast<std::shared_ptr<BlockTable>>(
            Context::current()->get("TABLE"));
        int x = luaL_checkinteger(state, -3);
        int y = luaL_checkinteger(state, -2);
        int z = luaL_checkinteger(state, -1);
        lua_pushstring(
            state,
            blockpack->getBlock(table->get(x, y, z).id)->getName().c_str());
        return 1;
}
int lua_getxsize(lua_State* state) {
        auto table = linb::any_cast<std::shared_ptr<BlockTable>>(
            Context::current()->get("TABLE"));
        lua_pushinteger(state, table->getXSize());
        return 1;
}
int lua_getysize(lua_State* state) {
        auto table = linb::any_cast<std::shared_ptr<BlockTable>>(
            Context::current()->get("TABLE"));
        lua_pushinteger(state, table->getYSize());
        return 1;
}
int lua_getzsize(lua_State* state) {
        auto table = linb::any_cast<std::shared_ptr<BlockTable>>(
            Context::current()->get("TABLE"));
        lua_pushinteger(state, table->getZSize());
        return 1;
}
}  // namespace planet