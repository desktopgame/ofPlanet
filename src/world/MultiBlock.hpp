#ifndef GAME_WORLD_MULTIBLOCK_HPP
#define GAME_WORLD_MULTIBLOCK_HPP
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../common/GLM.hpp"
namespace planet {

class MultiBlock {
       public:
        using Entry = std::pair<glm::vec3, char>;
        explicit MultiBlock();
        static MultiBlock fromFile(const std::string& file);
        static MultiBlock fromText(const std::string& text);
        void encode(char c, int blockId);
        int decode(char c) const;
        void stack(const std::string& layer);
        void stack(const std::vector<std::string> lines);
        std::vector<Entry> to3DData() const;
        int getXSize() const;
        int getYSize() const;
        int getZSize() const;
        glm::vec3 getSize() const;

       private:
        static int checkWidth(const std::vector<std::string> lines);
        static std::vector<std::string> split(const std::string& input,
                                              const char c);
        std::vector<std::string> stacks;
        std::unordered_map<char, int> blockMap;
        int xSize;
        int zSize;
};
}  // namespace planet
#endif