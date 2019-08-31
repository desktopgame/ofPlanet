#ifndef GEL_DEVICE_MODELMANAGER_HPP
#define GEL_DEVICE_MODELMANAGER_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "fbxsdk.h"

namespace gel {
class IModel;
/**
 * ModelManager is manage a `IModel`.
 * in now, supported .fbx
 */
class ModelManager {
       public:
        ModelManager();
        ~ModelManager();

        /**
         * load a .fbx file.
         * @param path
         */
        void loadFbx(const std::string& path);

        /**
         * unload model.
         * @param path
         */
        void unload(const std::string& path);

        /**
         * return a model.
         * @param path
         * @return
         */
        std::shared_ptr<IModel> getModel(const std::string& path) const;

       private:
        std::unordered_map<std::string, std::shared_ptr<IModel> > modelMap;
};
}  // namespace gel
#endif