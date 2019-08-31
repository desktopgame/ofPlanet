#ifndef GEL_SHADER_PLANEBATCH_HPP
#define GEL_SHADER_PLANEBATCH_HPP
#include <unordered_map>
#include "../gli.hpp"
#include "../util/Flag.hpp"
#include "NameRule.hpp"
#include "PlaneDraw.hpp"
namespace gel {
class PlaneBatch {
       public:
        explicit PlaneBatch(Shader& shader, const NameRule& nameRule);
        explicit PlaneBatch(Shader& shader);
        void init(float size);
        void destroy();
        void clear();
        PlaneDraw& get(GLuint id);
        void render();

       private:
        Flag initFlag;
        Shader& shader;
        NameRule nameRule;
        Plane plane;
        std::unordered_map<GLuint, PlaneDraw> map;
};
}  // namespace gel
#endif