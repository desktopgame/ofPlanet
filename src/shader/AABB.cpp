#include "AABB.hpp"

#include <algorithm>
#include <vector>
#include <ofGraphics.h>
#include <ofAppRunner.h>
#include <limits>

namespace planet {
	AABB::AABB() : init(false) {
	}
	AABB::AABB(std::vector<glm::vec3> verts)
		: min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
		max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()) {
		auto iter = verts.begin();
		while (iter != verts.end()) {
			auto v = (*iter);
			if (v.x < min.x) {
				min.x = v.x;
			}
			if (v.y < min.y) {
				min.y = v.y;
			}
			if (v.z < min.z) {
				min.z = v.z;
			}
			if (v.x > max.x) {
				max.x = v.x;
			}
			if (v.y > max.y) {
				max.y = v.y;
			}
			if (v.z > max.z) {
				max.z = v.z;
			}
			iter++;
		}
	}
	void AABB::update(const glm::vec3& vertex) {
		if (!init) {
			this->init = true;
			this->min = vertex;
			this->max = vertex;
			return;
		}
		min.x = std::min(min.x, vertex.x);
		min.y = std::min(min.y, vertex.y);
		min.z = std::min(min.z, vertex.z);

		max.x = std::max(max.x, vertex.x);
		max.y = std::max(max.y, vertex.y);
		max.z = std::max(max.z, vertex.z);
	}
	void AABB::draw(const glm::mat4 & matrix) {
		auto verts = getVertexes();
		auto iter = verts.begin();
		while (iter != verts.end()) {
			auto a = glm::vec4(*iter, 1);
			++iter;
			auto b = glm::vec4(*iter, 1);
			ofDrawLine(glm::vec3(matrix * a), glm::vec3(matrix * b));
			++iter;
		}
	}
	std::vector<glm::vec3> AABB::getVertexes() const {
		std::vector<glm::vec3> verts = std::vector<glm::vec3>{
			glm::vec3(min.x, min.y, max.z),
			glm::vec3(max.x, min.y, max.z),

			glm::vec3(min.x, max.y, max.z),
			glm::vec3(max.x, max.y, max.z),

			glm::vec3(min.x, min.y, min.z),
			glm::vec3(max.x, min.y, min.z),

			glm::vec3(min.x, max.y, min.z),
			glm::vec3(max.x, max.y, min.z),

			glm::vec3(min.x, min.y, min.z),
			glm::vec3(min.x, min.y, max.z),

			glm::vec3(min.x, max.y, min.z),
			glm::vec3(min.x, max.y, max.z),

			glm::vec3(max.x, min.y, min.z),
			glm::vec3(max.x, min.y, max.z),

			glm::vec3(max.x, max.y, min.z),
			glm::vec3(max.x, max.y, max.z),

			glm::vec3(min.x, min.y, min.z),
			glm::vec3(min.x, max.y, min.z),

			glm::vec3(max.x, min.y, min.z),
			glm::vec3(max.x, max.y, min.z),

			glm::vec3(min.x, min.y, max.z),
			glm::vec3(min.x, max.y, max.z),

			glm::vec3(max.x, min.y, max.z),
			glm::vec3(max.x, max.y, max.z),

		};
		return verts;
	}
	bool AABB::contains(const glm::vec3 & v) const {
		return (v.x >= min.x && v.x < max.x && v.y >= min.y && v.y < max.y && v.z >= min.z && v.z < max.z);
	}
	AABB AABB::transform(const glm::mat4 & matrix) const {
		auto verts = getVertexes();
		std::vector<glm::vec3> nverts;
		for (auto v : verts) {
			nverts.emplace_back(matrix * glm::vec4(v, 1));
		}
		return AABB(nverts);
	}
}  // namespace planet
