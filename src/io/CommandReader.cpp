#include "CommandReader.hpp"

#include "../text/Strings.hpp"
#include "File.hpp"
namespace planet {

CommandReader::CommandReader() : lines() {}

void CommandReader::parseFile(const std::string& file) {
        parseText(File::readAllText(file));
}

void CommandReader::parseText(const std::string& text) {
        this->lines = Strings::split(text, '\n');
}

std::string CommandReader::nextCommand(std::string&& outName,
                                       std::vector<std::string>& outArgs) {
        auto line = lines.at(0);
        line = Strings::lstrip(Strings::rstrip(line));
        // if empty line
        if (line.size() == 0) {
                outName = "";
                outArgs.clear();
                lines.erase(lines.begin());
                return line;
        }
        auto words = Strings::split(line, ' ');
        auto name = words[0];
        words.erase(words.begin());
        lines.erase(lines.begin());
        outName = name;
        outArgs.clear();
        for (auto e : words) outArgs.push_back(e);
        return line;
}

bool CommandReader::ready() const { return !lines.empty(); }
}  // namespace planet
