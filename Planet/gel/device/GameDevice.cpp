#include "GameDevice.hpp"
#include "../content/BmpPipeline.hpp"
#include "../content/ContentManager.hpp"
#include "../content/EchoPipeline.hpp"
#include "../content/FbxPipeline.hpp"
#include "../content/PngPipeline.hpp"
#include "../content/ProxyPipeline.hpp"
#include "../content/WavePipeline.hpp"

namespace gel {
GameDevice::GameDevice(const std::string& assetRootDir)
    : contentManager(std::make_shared<ContentManager>(assetRootDir)),
      audioManager(std::make_shared<AudioManager>()),
      modelManager(std::make_shared<ModelManager>()),
      textureManager(std::make_shared<TextureManager>()) {
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::WavePipeline> >(
                ".wav", audioManager));
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::BmpPipeline> >(
                ".bmp", textureManager));
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::PngPipeline> >(
                ".png", textureManager));
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::FbxPipeline> >(
                ".fbx", modelManager));
}
std::shared_ptr<GameDevice> GameDevice::make_shared(
    const std::string& assetRootDir) {
        return std::make_shared<GameDevice>(assetRootDir);
}
std::shared_ptr<ContentManager> GameDevice::getContentManager() const {
        return contentManager;
}
std::shared_ptr<AudioManager> GameDevice::getAudioManager() const {
        return audioManager;
}
std::shared_ptr<ModelManager> GameDevice::getModelManager() const {
        return modelManager;
}
std::shared_ptr<TextureManager> GameDevice::getTextureManager() const {
        return textureManager;
}
}  // namespace gel