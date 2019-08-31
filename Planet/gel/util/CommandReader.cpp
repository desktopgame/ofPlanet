#include "CommandReader.hpp"
#include "../util/io.hpp"
#include "../util/string.hpp"

namespace gel {
CommandReader::CommandReader() : lines() {}

void CommandReader::parseFile(const std::string& file) {
        parseText(readAllText(file));
}

void CommandReader::parseText(const std::string& text) {
        this->lines = gel::split(text, '\n');
}

std::string CommandReader::nextCommand(std::string&& outName,
                                       std::vector<std::string>& outArgs) {
        auto line = lines.at(0);
        line = lstrip(rstrip(line));
        // if empty line
        if (line.size() == 0) {
                outName = "";
                outArgs.clear();
                lines.erase(lines.begin());
                return line;
        }
        auto words = gel::split(line, ' ');
        auto name = words[0];
        words.erase(words.begin());
        lines.erase(lines.begin());
        outName = name;
        outArgs.clear();
        for (auto e : words) outArgs.push_back(e);
        return line;
}

bool CommandReader::ready() const { return !lines.empty(); }
}  // namespace gel