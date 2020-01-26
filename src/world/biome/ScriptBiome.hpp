#pragma once
#ifndef WORLD_BIOME_SCRIPTBIOME_HPP
#define WORLD_BIOME_SCRIPTBIOME_HPP
#include <ofxLua.h>
#include "BasicBiome.hpp"
#include "MultiBlock.hpp"

namespace planet {
using MultiBlockMap = std::unordered_map<std::string, MultiBlock>;
class ScriptBiome : public BasicBiome {
       public:
        explicit ScriptBiome(const std::string& filename);
        virtual void onGUI() override;

       protected:
        virtual bool isUseCallbacks() override;
        virtual void onBeginGenerateWorld(BlockTable& blockTable) override;
        virtual void onEndGenerateWorld(BlockTable& blockTable) override;
		virtual void onEndGenerateTerrain() override;
        virtual float onFixHeight(float y) override;
        virtual void onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                       int z) override;
        virtual void onGenerateWater(BlockTable& blockTable, int x, int y,
                                     int z) override;
        virtual void onGenerateStructures(BlockTable& blockTable) override;
        virtual void onGenerateCave(BlockTable& blockTable, int x, int y, int z,
                                    float noise) override;

       private:
        ofxLua::Interpreter lua;
        std::string mode;
        ofxLua::Context::Instance ctx;
        std::shared_ptr<BlockTable> table;
		std::shared_ptr<MultiBlockMap> multiBlockMap;
        std::unordered_map<std::string, ofxLua::Object> globals;
};

int lua_setblock(struct lua_State* state);
int lua_getblock(struct lua_State* state);
int lua_getxsize(struct lua_State* state);
int lua_getysize(struct lua_State* state);
int lua_getzsize(struct lua_State* state);
int lua_newstruct(struct lua_State* state);
int lua_genstruct(struct lua_State* state);
int lua_expandstruct(struct lua_State* state);
}  // namespace planet
#endif