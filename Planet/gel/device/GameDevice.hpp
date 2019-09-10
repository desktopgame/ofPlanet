#ifndef GEL_DEVICE_GAMEDEVICE_HPP
#define GEL_DEVICE_GAMEDEVICE_HPP
#include <memory>
#include "../pipeline/ContentManager.hpp"
namespace gel {
/**
 * GameDevice is object to reference a common game resources.
 * in general, instance of one only.
 */
class GameDevice {
       public:
        explicit GameDevice(const std::string& assetRootDir);
        static std::shared_ptr<GameDevice> make_shared(
            const std::string& assetRootDir);
        /**
         * return a ContentManager.
         * @return
         */
        std::shared_ptr<ContentManager> getContentManager() const;

       private:
        std::shared_ptr<ContentManager> contentManager;
};
}  // namespace gel
#endif