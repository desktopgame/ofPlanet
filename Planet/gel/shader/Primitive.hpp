#pragma once
#ifndef GEL_SHADER_PRIMITIVE_HPP
#define GEL_SHADER_PRIMITIVE_HPP
#include <vector>
#include <glm/glm.hpp>

namespace gel {
class Primitive {
public:
	static std::vector<glm::vec3> circle(const float y, const float scale);
private:
	Primitive() = delete;
	~Primitive() = delete;
};
}
#endif