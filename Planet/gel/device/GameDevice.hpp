#ifndef GEL_DEVICE_GAMEDEVICE_HPP
#define GEL_DEVICE_GAMEDEVICE_HPP
#include <memory>
#include "../content/ContentManager.hpp"
#include "AudioManager.hpp"
#include "ModelManager.hpp"
#include "TextureManager.hpp"
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

        /**
         * return a AudioManager.
         * @return
         */
        std::shared_ptr<AudioManager> getAudioManager() const;

        /**
         * return a ModelManager.
         * @return
         */
        std::shared_ptr<ModelManager> getModelManager() const;

        /**
         * return a TextureManager.
         * @return
         */
        std::shared_ptr<TextureManager> getTextureManager() const;

       private:
        std::shared_ptr<ContentManager> contentManager;
        std::shared_ptr<AudioManager> audioManager;
        std::shared_ptr<ModelManager> modelManager;
        std::shared_ptr<TextureManager> textureManager;
};
}  // namespace gel
#endif