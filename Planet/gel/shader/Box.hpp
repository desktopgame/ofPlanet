#pragma once
#ifndef GEL_SHADER_BOX_HPP
#define GEL_SHADER_BOX_HPP
#include "Shader.hpp"
#include "NameRule.hpp"
#include "Plane.hpp"
#include <memory>

namespace gel {
class Box {
public:
	explicit Box(const std::shared_ptr<Shader>& shader,
		const NameRule& nameRule);
	void init(const glm::vec4 color,
		glm::vec3 scale);
	void destroy();
	void draw();
private:
	Plane plane;
	glm::vec3 scale;
};
}
#endif