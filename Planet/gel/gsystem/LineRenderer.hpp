#ifndef GEL_GSYSTEM_LINERENDERER_HPP
#define GEL_GSYSTEM_LINERENDERER_HPP
#include <glm/glm.hpp>
#include <vector>
namespace gel {
class LineRenderer {
       public:
        explicit LineRenderer();
        void addPos(const glm::vec3& pos);
        void setPos(int index, const glm::vec3& pos);
        void clear();
        size_t size() const;
        void draw(int lineWidth);

       private:
        std::vector<glm::vec3> positions;
};
}  // namespace gel
#endif