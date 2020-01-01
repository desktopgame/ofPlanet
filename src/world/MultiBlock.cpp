#include "MultiBlock.hpp"

#include <cassert>
#include <sstream>
#include <stdexcept>

#include "../io/CommandReader.hpp"
#include "../io/File.hpp"
#include "../text/Strings.hpp"
#include "BlockPack.hpp"
#include "Block.hpp"
namespace planet {

MultiBlock::MultiBlock() : stacks(), xSize(), zSize(), blockMap() {}

MultiBlock MultiBlock::fromFile(const std::string& file) {
        return fromText(File::readAllText(file));
}

MultiBlock MultiBlock::fromText(const std::string& text) {
        CommandReader cmdr;
        MultiBlock mb;
        cmdr.parseText(text);
        std::string name;
        std::vector<std::string> args;
        std::vector<std::string> lines;
        std::vector<std::string> copy;
        while (cmdr.ready()) {
                cmdr.nextCommand(std::move(name), args);
                if (name == "begin") {
                        assert(args.size() == 0);
                        lines.clear();
                } else if (name == "set") {
                        assert(args.size() == 1);
                        lines.push_back(args[0]);
                } else if (name == "end") {
                        assert(args.size() == 0);
                        mb.stack(lines);
                        copy = lines;
                        lines.clear();
				} else if (name == "putID") {
					assert(args.size() == 2);
					auto key = args[0];
					auto id = std::stoi(args[1]);
					mb.encode(key[0], id);
				} else if(name == "putBlock") {
					assert(args.size() == 2);
					auto key = args[0];
					auto id = BlockPack::getCurrent()->getBlock(args[1])->getID();
					mb.encode(key[0], id);
                } else if (name == "dup") {
                        assert(args.size() == 1);
                        int count = std::stoi(args[0]);
                        for (int i = 0; i < count; i++) {
                                mb.stack(copy);
                        }
                }
        }
        return mb;
}

void MultiBlock::encode(char c, int blockId) { blockMap[c] = blockId; }

int MultiBlock::decode(char c) const { return blockMap.at(c); }

void MultiBlock::stack(const std::string& layer) {
        if (this->stacks.empty()) {
                auto lines = split(layer, '\n');
                this->xSize = checkWidth(lines);
                this->zSize = lines.size();
        }
        stacks.push_back(layer);
}

void MultiBlock::stack(const std::vector<std::string> lines) {
        if (this->stacks.empty()) {
                this->xSize = checkWidth(lines);
                this->zSize = lines.size();
        }
        std::stringstream ss;
        for (auto e : lines) {
                ss << e << std::endl;
        }
        stacks.push_back(ss.str());
}

std::vector<MultiBlock::Entry> MultiBlock::to3DData() const {
        std::vector<MultiBlock::Entry> ret;
        for (int i = 0; i < static_cast<int>(stacks.size()); i++) {
                int y = i;
                auto layer = stacks[i];
                auto lines = split(layer, '\n');
                for (int j = 0; j < static_cast<int>(lines.size()); j++) {
                        auto line = lines[j];
                        for (int k = 0; k < static_cast<int>(line.size()); k++) {
                                glm::vec3 v3(k, y, j);
                                ret.push_back(MultiBlock::Entry(v3, line[k]));
                        }
                }
        }
        return ret;
}

int MultiBlock::getXSize() const { return xSize; }

int MultiBlock::getYSize() const { return stacks.size(); }

int MultiBlock::getZSize() const { return zSize; }

glm::vec3 MultiBlock::getSize() const {
        return glm::vec3(getXSize(), getYSize(), getZSize());
}

// private
int MultiBlock::checkWidth(const std::vector<std::string> lines) {
        assert(!lines.empty());
        int w = lines[0].size();
        for (auto e : lines) {
                if (e.size() != w) {
                        throw std::logic_error("invalid text:" + e);
                }
        }
        return w;
}

std::vector<std::string> MultiBlock::split(const std::string& input,
                                           const char c) {
        return Strings::split(input, c);
}
}  // namespace planet
