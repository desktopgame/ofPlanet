#pragma once
#ifndef WORLD_WORLDGENERATESERVICE_HPP
#define WORLD_WORLDGENERATESERVICE_HPP
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <thread>

#include "../common/Random.hpp"
#include "BlockTable.hpp"
namespace planet {

class Biome;
class WorldGenerateService {
       public:
        explicit WorldGenerateService(int xSize, int ySize, int zSize);
        ~WorldGenerateService();

        std::future<BlockTable> get();

        void terminate();

        void setBufferSize(int bufferSize);
        int getBufferSize();

        int getXSize() const;
        int getYSize() const;
        int getZSize() const;

       private:
        int xSize, ySize, zSize;
        void doThreadRun();
        std::shared_ptr<Biome> sampleBiome();

        Random random;
        bool stop;
        int bufferSize;
        std::mutex stopMutex;

        std::vector<BlockTable> buf;
        std::mutex bufMutex;
        std::condition_variable bufCond;
        std::vector<std::thread> threads;
        std::vector<std::function<std::shared_ptr<Biome>()> > biomes;
};
}  // namespace planet
#endif