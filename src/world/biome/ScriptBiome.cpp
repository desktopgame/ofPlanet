#include "ScriptBiome.hpp"

#include "../MultiBlock.hpp"
#include "../Block.hpp"
#include "../BlockPack.hpp"
#include "../csvr/Parser.hpp"

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

using namespace ofxLua;
namespace planet {
ScriptBiome::ScriptBiome(const std::string& filename)
    : lua(), ctx(nullptr), table(nullptr), multiBlockMap() {
        lua.define("setblock", lua_setblock);
        lua.define("getblock", lua_getblock);
        lua.define("getxsize", lua_getxsize);
        lua.define("getysize", lua_getysize);
        lua.define("getzsize", lua_getzsize);
		lua.define("newstruct", lua_newstruct);
		lua.define("genstruct", lua_genstruct);
		lua.define("expandstruct", lua_expandstruct);
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
        ctx->set("TABLE", table);
		ctx->set("MB", multiBlockMap);
		ctx->set("HM", heightMap);
        // コールバックモードを決める
        std::vector<Variant> modeV = lua.call("start", std::vector<Object>{},
                 std::vector<Type>{T_STRING}).get();
        this->mode = modeV.at(0).string;
        if (this->mode != "default" && this->mode != "ignore") {
                std::cerr << "`" << mode << "` mode is undefined." << std::endl;
        }
}

void ScriptBiome::onEndGenerateWorld(BlockTable& blockTable) {
        lua.call("onPostGenerate", std::vector<Object>{}, std::vector<Type>{}).check();
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

void ScriptBiome::onEndGenerateTerrain() {
	ctx->set("HM", heightMap);
}

float ScriptBiome::onFixHeight(float y) {
        std::vector<Variant> r = lua.call("onFixHeight", std::vector<Object>{create(y)},
                 std::vector<Type>{T_NUMBER}).get();
        return static_cast<float>(r.at(0).number);
}

void ScriptBiome::onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                    int z) {
        std::vector<Variant> r = lua.call("onGenerateTerrain",
                 std::vector<Object>{create(x), create(y), create(z)},
                 std::vector<Type>{}).get();
}

void ScriptBiome::onGenerateWater(BlockTable& blockTable, int x, int y, int z) {
        std::vector<Variant> r = lua.call("onGenerateWater",
                 std::vector<Object>{create(x), create(y), create(z)},
                 std::vector<Type>{}).get();
}

void ScriptBiome::onGenerateStructures(BlockTable& blockTable) {
        std::vector<Variant> r = lua.call("onGenerateStructures", std::vector<Object>{},
                 std::vector<Type>{}).get();
}

void ScriptBiome::onGenerateCave(BlockTable& blockTable, int x, int y, int z,
                                 float noise) {
        std::vector<Variant> r = lua.call("onGenerateCave",
                 std::vector<Object>{create(x), create(y), create(z),
                                     create(static_cast<double>(noise))},
			std::vector<Type>{}).get();
}

int lua_setblock(lua_State* state) {
        auto blockpack = BlockPack::getCurrent();
        auto table = 
            Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
        int x = luaL_checkinteger(state, -4);
        int y = luaL_checkinteger(state, -3);
        int z = luaL_checkinteger(state, -2);
		x = std::max(0, std::min(x, table->getXSize()-1));
		y = std::max(0, std::min(y, table->getYSize()-1));
		z = std::max(0, std::min(z, table->getZSize()-1));
        std::string name = luaL_checkstring(state, -1);
        int id = BlockPack::getCurrent()->getBlockIndexForName(name);
        table->set(x, y, z, BlockPrefab(id, false));
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
			for (int k = 0; k < static_cast<int>(line.size()); k++) {
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
	using HeightMapT = std::unordered_map<glm::ivec2, int, hidden::Vec2HashFunc, hidden::Vec2HashFunc>;
	auto table = Context::top()->get<std::shared_ptr<BlockTable> >("TABLE");
	auto mbmap = Context::top()->get<std::shared_ptr<MultiBlockMap> >("MB");
	auto hmap = Context::top()->get<HeightMapT >("HM");
	auto terrain = table->getTerrain();
	// 構造名を取得する
	std::string name = luaL_checkstring(state, -1);
	auto& mb = mbmap->at(name);
	glm::ivec3 mbSize;
	multiBlock3DSize(mb, mbSize);

	int genLimit = 5;
	int testCount = 30000;
	Random random;

	int success = 0;
	for (int n = 0; n < testCount; n++) {
		int x = static_cast<int>(
			random.generate(0, table->getXSize() - 1));
		int z = static_cast<int>(
			random.generate(0, table->getZSize() - 1));
		int y = hmap[glm::ivec2(x, z)];
		bool fail = false;
		for (int l = 0; l < mbSize.x; l++) {
			for (int p = 0; p < mbSize.z; p++) {
				glm::ivec2 key((x + p, z + l));
				if (!hmap.count(key) ||
					y != hmap[key]) {
					fail = true;
					break;
				}
			}
			if (fail) {
				break;
			}
		}
		if (!fail) {
			success++;
			table->expand(x + 1, y + 1, z + 1, mb);
		}
		if (success >= genLimit) {
			break;
		}
	}

//	table->expand(mb);
	return 0;
}

int lua_expandstruct(lua_State * state) {
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
}  // namespace planet