#ifndef GEL_CONTENT_PROXYPIPELINE_H
#define GEL_CONTENT_PROXYPIPELINE_H
#include <memory>
#include <string>
#include <unordered_map>
#include "IContentPipeline.hpp"

#define STAT_NOTLOADED (1)
#define STAT_BACKGROUND (2)
#define STAT_GL (3)

namespace gel {
template <typename T>
class ProxyPipeline : public IContentPipeline {
       public:
        template <typename... Args>
        ProxyPipeline(const std::string& extension, Args... args);
        bool accept(const std::string& path) override;
        void load(const std::string& path, Thread thread) override;
        void unload(const std::string& path) override;

       private:
        std::string extension;
        std::shared_ptr<T> proxy;
        std::unordered_map<std::string, unsigned int> statMap;
};
template <typename T>
template <typename... Args>
ProxyPipeline<T>::ProxyPipeline(const std::string& extension, Args... args)
    : statMap() {
        this->extension = extension;
        this->proxy = std::make_shared<T>(args...);
}
template <typename T>
bool ProxyPipeline<T>::accept(const std::string& path) {
        return (path.size() >= extension.size() &&
                path.find(extension, path.size() - extension.size()) !=
                    std::string::npos);
}

template <typename T>
void ProxyPipeline<T>::load(const std::string& path, Thread thread) {
        // T requires loadOn method
        if (!statMap.count(path)) {
                statMap[path] = STAT_NOTLOADED;
        }
        switch (thread) {
                case Thread::OnBackground:
                        if (statMap[path] != STAT_NOTLOADED) {
                                throw std::logic_error(path +
                                                       " is already loaded");
                        }
                        statMap[path] = STAT_BACKGROUND;
                        break;
                case Thread::OnGL:
                        if (statMap[path] != STAT_BACKGROUND) {
                                throw std::logic_error(path +
                                                       " is already loaded");
                        }
                        statMap[path] = STAT_GL;
                        break;
        }
        proxy->load(path, thread);
}

template <typename T>
void ProxyPipeline<T>::unload(const std::string& path) {
        // T requires unload method
        proxy->unload(path);
}
}  // namespace gel
#endif