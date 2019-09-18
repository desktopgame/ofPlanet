#include "Primitive.hpp"
namespace gel {
	std::vector<glm::vec3> Primitive::circle(const float y, const float scale)
	{
		std::vector<glm::vec3> v;
		for (int i = 0; i < 360; i += 2) {
			float rad = (float)i * (3.14f / 180.0f);
			float x = std::cos(rad) * scale;
			float z = std::sin(rad) * scale;
			v.push_back(glm::vec3(x, y, z));
		}
		return v;
	}
}