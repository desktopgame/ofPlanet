#ifndef GEL_DEVICE_IMODEL_HPP
#define GEL_DEVICE_IMODEL_HPP
#include <memory>
#include <string>
#include "../math/AABB.hpp"

namespace gel {
class IRModel;
class IModel {
       public:
        IModel() = default;
        virtual ~IModel() = default;
        virtual void load(const std::string& path) = 0;
        virtual void unload(const std::string& path) = 0;
        virtual std::shared_ptr<IRModel> getIRModel() const = 0;
};
}  // namespace gel
#endif