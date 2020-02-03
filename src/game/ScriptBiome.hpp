#pragma once
#ifndef WORLD_BIOME_SCRIPTBIOME_HPP
#define WORLD_BIOME_SCRIPTBIOME_HPP
#include <ofxLua.h>

#include <ofxPlanet.h>

namespace planet {
class ScriptBiome : public ofxPlanet::BasicBiome {
       public:
        explicit ScriptBiome(const std::string& filename);
		~ScriptBiome();
        void onGUI();

       protected:
        virtual bool isUseCallbacks() override;
        virtual void onBeginGenerateWorld(ofxPlanet::BlockTable& blockTable) override;
        virtual void onEndGenerateWorld(ofxPlanet::BlockTable& blockTable) override;
        virtual void onEndGenerateTerrain(ofxPlanet::BlockTable& blockTable) override;
        virtual float onFixHeight(float y) override;
        virtual void onGenerateTerrain(ofxPlanet::BlockTable& blockTable, int x, int y,
                                       int z) override;
        virtual void onGenerateStructures(ofxPlanet::BlockTable& blockTable) override;
        virtual void onGenerateCave(ofxPlanet::BlockTable& blockTable, int x, int y, int z,
                                    float noise) override;

       private:
        ofxLua::Interpreter lua;
        std::string mode;
        ofxLua::Context::Instance ctx;
		ofxPlanet::BlockTable* table;
        std::unordered_map<std::string, ofxLua::Object> globals;
};

int lua_setblock(struct lua_State* state);
int lua_putblock(struct lua_State* state);
int lua_getblock(struct lua_State* state);
int lua_setblockrange(struct lua_State* state);
int lua_putblockrange(struct lua_State* state);
int lua_replaceblockrange(struct lua_State* state);
int lua_getxsize(struct lua_State* state);
int lua_getysize(struct lua_State* state);
int lua_getzsize(struct lua_State* state);
int lua_newstruct(struct lua_State* state);
int lua_genstruct(struct lua_State* state);
int lua_expandstruct(struct lua_State* state);
int lua_setweight(struct lua_State* state);
int lua_getweight(struct lua_State* state);
int lua_setweightrange(struct lua_State* state);
}  // namespace planet
#endif