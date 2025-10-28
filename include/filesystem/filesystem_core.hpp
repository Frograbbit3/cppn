#pragma once
#include "SDL2/SDL_filesystem.h"
#include <SDL2/SDL_stdinc.h>
#include <fmt/core.h>
#include <stdexcept>
#include <string>
#include "stdio.h"
#include <vector>

#ifdef __unix
    #include <sys/stat.h>
    #include <dirent.h>
    #include <unistd.h>
#endif


namespace CPPN {
    namespace FileSystem {
        static std::string SAVE_PATH;
        static std::string RESOURCE_PATH;
        static void Init(std::string companyName, std::string gameName) {
            char* save= SDL_GetPrefPath(companyName.c_str(), gameName.c_str());
            SAVE_PATH = save;
            SDL_free(save);

            char* core=SDL_GetBasePath();
            RESOURCE_PATH=core;
            SDL_free(core);
        }

        std::string JoinPaths(const std::vector<std::string>& parts) {
            if (parts.empty()) return "";

            std::string result = parts[0];
            for (size_t i = 1; i < parts.size(); ++i) {
                if (result.back() != '/' && result.back() != '\\') {
                #ifdef _WIN32
                    result += '\\';
                #else
                    result += '/';
                #endif
                }

                std::string next = parts[i];
                // Strip leading slashes from the next part
                while (!next.empty() && (next.front() == '/' || next.front() == '\\'))
                    next.erase(next.begin());

                result += next;
            }

            return result;
        }

        std::string AbsoluteResourcePath(std::string rel) {
            return JoinPaths({RESOURCE_PATH, rel});
        }
        std::string AbsoluteSavesPath(std::string rel) {
            return JoinPaths({SAVE_PATH, rel});
        }
        bool IsAbsolutePath(const std::string& path) {
            if (path.compare(0, RESOURCE_PATH.size(), RESOURCE_PATH) == 0 ||
                path.compare(0, SAVE_PATH.size(), SAVE_PATH) == 0) {
                return true;
            }
            return false;
        }
        bool FileExists(std::string path) {
            if (!IsAbsolutePath(path)) {
                path=AbsoluteResourcePath(path);
            }
            FILE* f = fopen(path.c_str(), "r");
            if(f) {
                fclose(f);
                return true;
            }
            return false;
        }

        std::string OpenFileAsText(std::string path) {
            if (!FileExists(path)) {
                throw std::runtime_error("File not found");
            }
            if (!IsAbsolutePath(path)) {
                path=AbsoluteResourcePath(path);
            }
            std::string text;
            FILE* file = fopen(path.c_str(), "r");
            if (!file) {
                throw std::runtime_error("Unable to read file!");
            }
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file)) {
                text += buffer;
            }

            fclose(file);
            return text;

        }

    }
}
