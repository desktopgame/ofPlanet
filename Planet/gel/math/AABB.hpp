#ifndef GEL_MATH_AABB_HPP
#define GEL_MATH_AABB_HPP
#include <glm/glm.hpp>
#include <vector>
//#include "Matrix4.hpp"
//#include "Vector3.hpp"
namespace gel {
/**
 * AABB is Axis-Aligned Bounding Box.
 */
class AABB {
       public:
        explicit AABB();
        explicit AABB(float minX, float minY, float minZ, float maxX,
                      float maxY, float maxZ);
        explicit AABB(std::vector<glm::vec3> vertexes);
        /**
         * return a all points.
         * @return
         */
        std::vector<glm::vec3> points() const;
        /**
         * draw a quad of AABB.
         */
        void drawQuad() const;
        /**
         * draw a frame of AABB.
         * @param lineWidth
         */
        void drawFrame(int lineWidth = 10) const;
        /**
         * return a size of AABB.
         * @return
         */
        glm::vec3 getSize() const;
        float getSizeX() const;
        float getSizeY() const;
        float getSizeZ() const;
        AABB scaled(const glm::vec3& scale) const;
        AABB transform(const glm::mat4& m) const;
        static bool isIntersects(const AABB& a, const AABB& b);
        glm::vec3 clamp(const glm::vec3& pos) const;
        glm::vec3 center;
        glm::vec3 radius;
        float minX;
        float minY;
        float minZ;
        float maxX;
        float maxY;
        float maxZ;

       private:
        void init();
};
}  // namespace gel
#endif