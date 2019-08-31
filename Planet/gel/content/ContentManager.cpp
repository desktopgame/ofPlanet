#include "ContentManager.hpp"
#include <algorithm>
#include <iostream>
#include "IContentPipeline.hpp"
#if __APPLE__
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#pragma warning(disable : 4996)

namespace gel {
ContentManager::ContentManager(const std::string& rootDir)
    : rootDir(rootDir), files(), pipes() {
        collect(rootDir, 0);
}

void ContentManager::add(const std::shared_ptr<IContentPipeline>& pipe) {
        this->pipes.push_back(pipe);
}

void ContentManager::remove(const std::shared_ptr<IContentPipeline>& pipe) {
        //        auto iter = std::find(pipes.begin(), pipes.end(),
        //                              [pipe](auto& e) -> bool { return e ==
        //                              pipe; });
        // pipes.erase(iter);
}

void ContentManager::load() {
        std::for_each(files.begin(), files.end(), [&](auto file) {
                std::for_each(pipes.begin(), pipes.end(), [&](auto p) {
                        if (p->accept(file)) {
                                p->load(file);
                        }
                });
        });
}

void ContentManager::unload() {
        std::for_each(files.begin(), files.end(), [&](auto file) {
                std::for_each(pipes.begin(), pipes.end(), [&](auto p) {
                        if (p->accept(file)) {
                                p->unload(file);
                        }
                });
        });
}

// private
void ContentManager::collect(const std::string& dir, int depth) {
#if __APPLE__
        auto dp = ::opendir(dir.c_str());
        struct dirent* de;
        if (dp == NULL) {
                return;
        }
        struct stat path_stat;
        for (de = ::readdir(dp); de != NULL; de = ::readdir(dp)) {
                std::string name = de->d_name;
                if (name == "." || name == "..") {
                        continue;
                }
                name = dir + '/' + name;
                ::stat(name.c_str(), &path_stat);
                if (S_ISREG(path_stat.st_mode)) {
                        files.push_back(name);
                } else {
                        collect(name, depth + 1);
                }
        }
        ::closedir(dp);
#elif _WIN32
        HANDLE hFind;
        WIN32_FIND_DATA fdFile;

        FILETIME ft;
        SYSTEMTIME st;

        char target[MAX_PATH + 1];
        char curpath[MAX_PATH + 1];
        char modifiedtime[MODIFIEDTM_LEN + 1];
        bool canread = true;

        // fprintf( stdout, "curdepth %d\n", curdepth );

        if (depth > 42) {
                return;
        }

        depth++;

        // DBGPRINT(path.c_str());

        if (static_cast<int>(dir.length()) > MAX_PATH) {
                fprintf(stderr, "Length of File Path exceeded max value !");
                canread = false;
        } else {
                sprintf(target, "%s\\*", dir.c_str());
                hFind = FindFirstFile(target, &fdFile);

                if (hFind == INVALID_HANDLE_VALUE) {
                        return;
                }
                do {
                        if (!strcmp(fdFile.cFileName, ".") ||
                            !strcmp(fdFile.cFileName, "..")) {
                                continue;
                        }
                        if (!isSystemFileWindows(fdFile.dwFileAttributes)) {
                                continue;
                        }
                        // if the file is a dirctory
                        if ((fdFile.dwFileAttributes &
                             FILE_ATTRIBUTE_DIRECTORY) ==
                            FILE_ATTRIBUTE_DIRECTORY) {
                                sprintf(curpath, "%s/%s", dir.c_str(),
                                        fdFile.cFileName);
                                // recursive call
                                // v(path + "/" + fdFile.cFileName, false);
                                collect(curpath, depth + 1);
                        } else {
                                // v(path + "/" + fdFile.cFileName, true);
                                sprintf(curpath, "%s/%s", dir.c_str(),
                                        fdFile.cFileName);
                                // convert time
                                FileTimeToLocalFileTime(&fdFile.ftLastWriteTime,
                                                        &ft);
                                FileTimeToSystemTime(&ft, &st);
                                sprintf(modifiedtime,
                                        "%04d%02d%02d %02d%02d%02d", st.wYear,
                                        st.wMonth, st.wDay, st.wHour,
                                        st.wMinute, st.wSecond);
                                // print a regular file etc ...
                                // fprintf(stdout, "%s %s\n", modifiedtime,
                                // curpath);
                                files.push_back(curpath);
                        }
                } while (FindNextFile(hFind, &fdFile));
                FindClose(hFind);
        }
#endif
}

bool ContentManager::isSystemFileWindows(DWORD d) {
        return ((d & FILE_ATTRIBUTE_HIDDEN)

                    != FILE_ATTRIBUTE_HIDDEN

                && (d & FILE_ATTRIBUTE_SYSTEM)

                       != FILE_ATTRIBUTE_SYSTEM

                && (d & FILE_ATTRIBUTE_TEMPORARY)

                       != FILE_ATTRIBUTE_TEMPORARY);
}
}  // namespace gel