#pragma once
#ifndef INPUT_KEYBOARDBUFFER_HPP
#define INPUT_KEYBOARDBUFFER_HPP
#include <unordered_map>
#include <vector>
namespace planet {

class KeyboardBuffer {
       public:
        explicit KeyboardBuffer();
        std::unordered_map<int, bool> prevKeymap;
        std::unordered_map<int, bool> currKeymap;
        static const std::vector<int> GLFW_KEY_LIST;
};
}  // namespace planet
#endif