#ifndef GEL_DEVICE_IMODEL_HPP
#define GEL_DEVICE_IMODEL_HPP
#include <memory>
#include <string>
#include "../math/AABB.hpp"
#include "IAsset.hpp"

namespace gel {
class IRModel;
class IModel : public IAsset {
       public:
        IModel() = default;
        virtual ~IModel() = default;
        virtual std::shared_ptr<IRModel> getIRModel() const = 0;
};
}  // namespace gel
#endif