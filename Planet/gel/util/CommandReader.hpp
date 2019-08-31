#ifndef GEL_UTIL_COMMANDREADER_HPP
#define GEL_UTIL_COMMANDREADER_HPP
#include <string>
#include <vector>

namespace gel {
class CommandReader {
       public:
        explicit CommandReader();
        void parseFile(const std::string& file);
        void parseText(const std::string& text);

        std::string nextCommand(std::string&& outName,
                                std::vector<std::string>& outArgs);
        bool ready() const;

       private:
        std::vector<std::string> lines;
};
}  // namespace gel
#endif