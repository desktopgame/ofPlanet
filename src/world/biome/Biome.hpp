#pragma once
#ifndef WORLD_BIOME_BIOME_HPP
#define WORLD_BIOME_BIOME_HPP
#include <memory>
#include <ofxImGui.h>
namespace planet {

class BlockTable;
class World;
class Biome {
       public:
        Biome() = default;
        virtual ~Biome() = default;
		//FIXME:ここに定義するべきでない
		virtual void onGUI() = 0;
        virtual void generate(BlockTable& blockTable) = 0;

       private:
};
}  // namespace planet
#endif