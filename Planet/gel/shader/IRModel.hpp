#pragma once
#ifndef GEL_SHADER_IRMODEL_HPP
#define GEL_SHADER_IRMODEL_HPP
#include <memory>

namespace gel {
class IRMesh;

class IRModel {
       public:
        explicit IRModel();
        ~IRModel();
        std::shared_ptr<IRMesh> getMesh() const;

       private:
        std::shared_ptr<IRMesh> mesh;
};
}  // namespace gel
#endif