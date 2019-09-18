#pragma once
#ifndef GEL_SHADER_PRIMITIVE_HPP
#define GEL_SHADER_PRIMITIVE_HPP
#include <glm/glm.hpp>
#include <vector>

namespace gel {
class Primitive {
       public:
        static std::vector<glm::vec3> circleLine(const float y,
                                                 const float scale);

       private:
        Primitive() = delete;
        ~Primitive() = delete;
};
}  // namespace gel
#endif