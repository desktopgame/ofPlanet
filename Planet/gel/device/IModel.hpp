#ifndef GEL_DEVICE_IMODEL_HPP
#define GEL_DEVICE_IMODEL_HPP
#include <string>
#include "../math/AABB.hpp"

namespace gel {
class IModel {
       public:
        IModel() = default;
        virtual ~IModel() = default;
        virtual void load(const std::string& path) = 0;
        virtual void unload(const std::string& path) = 0;
        virtual void draw() = 0;
        virtual AABB getAABB() const = 0;
};
}  // namespace gel
#endif