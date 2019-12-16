#include "WorldGenerateService.hpp"

#include <chrono>

#include "../common/Stopwatch.hpp"
#include "biome/HillBiome.hpp"
#include "biome/PlainBiome.hpp"
#include "ofApp.h"
namespace planet {

WorldGenerateService::WorldGenerateService(int xSize, int ySize, int zSize)
    : random(),
      stop(false),
      stopMutex(),
      buf(),
      bufMutex(),
      bufCond(),
      threads(),
      biomes(),
      bufferSize(10),
      xSize(xSize),
      ySize(ySize),
      zSize(zSize) {
        // biomes.emplace_back(std::make_shared<DesertBiome>());
        biomes.emplace_back([]() { return std::make_shared<PlainBiome>(); });
        biomes.emplace_back([]() { return std::make_shared<HillBiome>(); });
        // biomes.emplace_back([]() { return std::make_shared<MoonBiome>(); });
        for (int i = 0; i < 5; i++) {
                threads.emplace_back(std::thread(
                    std::bind(&WorldGenerateService::doThreadRun, this)));
        }
}

WorldGenerateService::~WorldGenerateService() {
        {
                std::lock_guard<std::mutex> stopLock(stopMutex);
                if (stop) {
                        return;
                }
        }
        terminate();
}

std::future<BlockTable> WorldGenerateService::get() {
        return std::async(std::launch::async, [&]() -> BlockTable {
                Stopwatch sw("RequireWorld");
                sw.start();
                std::unique_lock<std::mutex> bufLock(bufMutex);
                bufCond.wait(bufLock);
                BlockTable t = buf.at(0);
                buf.erase(buf.begin());
                sw.stop();
                sw.log<std::chrono::milliseconds>();
                return t;
        });
}
void WorldGenerateService::terminate() {
        {
                std::lock_guard<std::mutex> stopLock(stopMutex);
                this->stop = true;
        }
        for (std::thread& t : threads) {
                if (t.joinable()) {
                        t.join();
                }
        }
}
void WorldGenerateService::setBufferSize(int bufferSize) {
        std::lock_guard<std::mutex> bufLock(bufMutex);
        this->bufferSize = bufferSize;
}
int WorldGenerateService::getBufferSize() {
        std::lock_guard<std::mutex> bufLock(bufMutex);
        return bufferSize;
}
int WorldGenerateService::getXSize() const { return xSize; }
int WorldGenerateService::getYSize() const { return ySize; }
int WorldGenerateService::getZSize() const { return zSize; }
// private
void WorldGenerateService::doThreadRun() {
        while (true) {
                {
                        std::lock_guard<std::mutex> stopLock(stopMutex);
                        if (stop) {
                                break;
                        }
                }
                {
                        std::lock_guard<std::mutex> bufLock(bufMutex);
                        if (!buf.empty()) {
                                bufCond.notify_one();
                                std::this_thread::yield();
                        }
                        if (buf.size() > bufferSize) {
                                continue;
                        }
                }
                {
                        BlockTable table = BlockTable(xSize, ySize, zSize);
                        Stopwatch sw("WorldGenerateService");
                        sw.start();
                        sampleBiome()->generate(table);
                        sw.stop();
                        sw.log<std::chrono::milliseconds>();

                        std::lock_guard<std::mutex> bufLock(bufMutex);
                        buf.emplace_back(table);
                        bufCond.notify_one();
                }
                {
                        std::lock_guard<std::mutex> stopLock(stopMutex);
                        if (stop) {
                                break;
                        }
                }
                std::this_thread::yield();
        }
}

std::shared_ptr<Biome> WorldGenerateService::sampleBiome() {
        assert(!biomes.empty());
        return biomes.at(
            random.generate(0, static_cast<int>(biomes.size()) - 1))();
}
}  // namespace planet
