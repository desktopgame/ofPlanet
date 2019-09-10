#pragma once
#ifndef GEL_ASSET_ASSETDATABASE_HPP
#define GEL_ASSET_ASSETDATABASE_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include "IAsset.hpp"

namespace gel {
class AssetDatabase {
       public:
        template <typename T, typename... Args>
        static std::shared_ptr<T> newAsset(const std::string& path,
                                           Args... args);

        static void addAsset(const std::string& path,
                             const std::shared_ptr<IAsset>& asset);

        template <typename T>
        static std::shared_ptr<T> getAsset(const std::string& path);

        template <typename T, typename... Args>
        static std::shared_ptr<T> getOrNewAsset(const std::string& path,
                                                Args... args);

        template <typename T>
        static std::shared_ptr<T> removeAsset(const std::string& path);

       private:
        AssetDatabase() = delete;
        ~AssetDatabase() = delete;
        static std::unordered_map<std::string, std::shared_ptr<IAsset> > assets;
};

template <typename T, typename... Args>
inline std::shared_ptr<T> AssetDatabase::newAsset(const std::string& path,
                                                  Args... args) {
        auto ret = std::make_shared<T>(args...);
        addAsset(path, std::static_pointer_cast<IAsset>(ret));
        return ret;
}

template <typename T>
inline std::shared_ptr<T> AssetDatabase::getAsset(const std::string& path) {
        if (assets.count(path)) {
                return std::static_pointer_cast<T>(assets[path]);
        }
        return nullptr;
}

template <typename T, typename... Args>
inline std::shared_ptr<T> AssetDatabase::getOrNewAsset(const std::string& path,
                                                       Args... args) {
        if (assets.count(path)) {
                return std::static_pointer_cast<T>(assets[path]);
        }
        return newAsset<T>(path, args...);
}

template <typename T>
inline std::shared_ptr<T> AssetDatabase::removeAsset(const std::string& path) {
        std::shared_ptr<T> ret = nullptr;
        if (assets.count(path)) {
                ret = std::static_pointer_cast<T>(assets[path]);
                assets.erase(path);
        }
        return ret;
}
}  // namespace gel
#endif