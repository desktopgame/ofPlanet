#include "GameDevice.hpp"
#include "../content/BmpPipeline.hpp"
#include "../content/ContentManager.hpp"
#include "../content/EchoPipeline.hpp"
#include "../content/FbxPipeline.hpp"
#include "../content/JpegPipeline.hpp"
#include "../content/PngPipeline.hpp"
#include "../content/ProxyPipeline.hpp"
#include "../content/WavePipeline.hpp"

namespace gel {
GameDevice::GameDevice(const std::string& assetRootDir)
    : contentManager(std::make_shared<ContentManager>(assetRootDir)) {
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::WavePipeline> >(".wav"));
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::BmpPipeline> >(".bmp"));
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::PngPipeline> >(".png"));
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::FbxPipeline> >(".fbx"));
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::JpegPipeline> >(".jpeg"));
        contentManager->add(
            std::make_shared<gel::ProxyPipeline<gel::JpegPipeline> >(".jpg"));
}
std::shared_ptr<GameDevice> GameDevice::make_shared(
    const std::string& assetRootDir) {
        return std::make_shared<GameDevice>(assetRootDir);
}
std::shared_ptr<ContentManager> GameDevice::getContentManager() const {
        return contentManager;
}
}  // namespace gel