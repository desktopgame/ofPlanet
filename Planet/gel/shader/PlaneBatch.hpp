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
        explicit PlaneBatch(const std::shared_ptr<Shader>& shader,
                            const NameRule& nameRule);
        explicit PlaneBatch(const std::shared_ptr<Shader>& shader);
        void init(float size);
        void destroy();
        void clear();
        PlaneDraw& get(GLuint id);
        void render();

       private:
        Flag initFlag;
        std::shared_ptr<Shader> shader;
        NameRule nameRule;
        Plane plane;
        std::unordered_map<GLuint, PlaneDraw> map;
};
}  // namespace gel
#endif