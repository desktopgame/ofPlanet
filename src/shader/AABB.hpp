#pragma once
#ifndef SHADER_AABB_HPP
#define SHADER_AABB_HPP
#include "../common/GLM.hpp"
#include <vector>
namespace planet {

	struct AABB {
		explicit AABB();
		explicit AABB(std::vector<glm::vec3> verts);
		void update(const glm::vec3& vertex);
		void draw(const glm::mat4& matrix);
		std::vector<glm::vec3> getVertexes() const;
		bool contains(const glm::vec3& v) const;
		AABB transform(const glm::mat4& matrix) const;
		glm::vec3 min;
		glm::vec3 max;
		bool init;
	};
}  // namespace planet
#endif