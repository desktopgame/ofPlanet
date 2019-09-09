#ifndef GEL_CONTENT_CONTENTMANAGER_HPP
#define GEL_CONTENT_CONTENTMANAGER_HPP
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
#include <memory>
#include <string>
#include <vector>

namespace gel {
class IContentPipeline;
/**
 * ContentManager is list of ContentPipeline.
 */
class ContentManager {
       public:
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
        void load(const std::string& path);
        /**
         * load a all asset.
         */
        void load();
        /**
         * unload a all asset.
         */
        void unload();

       private:
        void collect(const std::string& dir, int depth);
        static bool isSystemFileWindows(DWORD d);
        std::string rootDir;
        std::vector<std::string> files;
        std::vector<std::shared_ptr<IContentPipeline> > pipes;
};
}  // namespace gel
#endif