#ifndef GEL_SHADER_PLANEDRAW_HPP
#define GEL_SHADER_PLANEDRAW_HPP
#include <unordered_map>
#include <vector>
#include "../util/Flag.hpp"
#include "Buffer.hpp"
#include "Plane.hpp"
namespace gel {
class PlaneDraw {
       public:
        explicit PlaneDraw();
        void init();
        void destroy();
        void clear();
        void add(PlaneType type, float x, float y, float z, float w);
        void add(PlaneType type, glm::vec4 pos);
        void update();
        void render(Plane& plane, const NameRule nameRule);

       private:
        Flag initFlag;
        std::unordered_map<PlaneType, Buffer<float> > map;
};
}  // namespace gel
#endif