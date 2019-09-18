#ifndef GEL_PIPELINE_CONTENTMANAGER_HPP
#define GEL_PIPELINE_CONTENTMANAGER_HPP
#if _WIN32
#include <Windows.h>
#define SH_PATH_MAX 255
#define MODIFIEDTM_LEN 15 + 1 /* "YYYYMMDD HHMMSS" */

#ifdef _DEBUG
#define DBGPRINT(VALUE) \
        { printf("[DEBUG][Line %05d] %s [%s]\n", __LINE__, #VALUE, VALUE); }
#else
#define DBGPRINT(VALUE)
#endif
#endif
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "../Thread.hpp"
#include "../signal/Signal.hpp"

namespace gel {
class IContentPipeline;
struct ContentLoadEvent {
        std::string path;
};
struct ContentUnloadEvent {
        std::string path;
};
/**
 * ContentManager is list of ContentPipeline.
 */
class ContentManager {
       public:
        using LoadFunc = std::function<void(const ContentLoadEvent)>;
        using UnloadFunc = std::function<void(const ContentUnloadEvent)>;
        /**
         * create files cache by specifid path.
         * still not running content load on construct.
         * need call loadXXX function for real content laod
         * @param rootDir
         */
        ContentManager(const std::string& rootDir);
        /**
         * add pipeline.
         * @param pipe
         */
        void add(const std::shared_ptr<IContentPipeline>& pipe);
        /**
         * remove pipeline.
         * @param pipe
         */
        void remove(const std::shared_ptr<IContentPipeline>& pipe);

        /**
         * load a asset.
         * and, remove specified file from files cache.
         * @param path
         */
        void loadFully(const std::string& path);

		/**
         * load a asset.
         * and, remove specified file from files cache.
		 * @param pred
		 */
		void loadFully(std::function<bool(const std::string&)> pred);

        /**
         * load a all asset.
         * @param thread
         */
        void load(Thread thread);
        /**
         * unload a all asset.
         */
        void unload();

        /**
         * return a content count.
         * @return
         */
        int getContentCount() const;

        /**
         * @return
         */
        Signal<ContentLoadEvent>& onContentLoad();

        /**
         * @return
         */
        Signal<ContentUnloadEvent>& onContentUnload();

       private:
        void collect(const std::string& dir, int depth);
        static bool isSystemFileWindows(DWORD d);
        std::string rootDir;
        std::vector<std::string> files;
        std::vector<std::shared_ptr<IContentPipeline> > pipes;
        Signal<ContentLoadEvent> contentLoadSlot;
        Signal<ContentUnloadEvent> contentUnoadSlot;
};
}  // namespace gel
#endif