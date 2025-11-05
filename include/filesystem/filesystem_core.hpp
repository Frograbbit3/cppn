#pragma once
#include "SDL2/SDL_filesystem.h"
#include "SDL2/SDL_rwops.h"
#include "utils/other_utils.hpp"
#include <SDL2/SDL_stdinc.h>
#include <any>
#include <cstddef>
#define FMT_HEADER_ONLY
#include "../fmt/core.h"
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include "stdio.h"
#include "zlib.h"
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
        
        /*
            Sets up the file system, allowing saving and loading assets.
            @param companyName The name of your company, used for saving.
            @param gameName The name of your game, used for saving.
        */
        static void Init(std::string companyName, std::string gameName) {
            char* save= SDL_GetPrefPath(companyName.c_str(), gameName.c_str());
            SAVE_PATH = save;
            SDL_free(save);

            char* core=SDL_GetBasePath();
            RESOURCE_PATH=core;
            SDL_free(core);
        }

        /*
            Combines two or more parts of a path into one. (folder1, folder2) --> folder1/folder2
            @param parts The segments of paths

        */
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


        /*
            Will take a relative path (so file.txt) and convert it to an absolute path to the assets based on the system (/app/file.txt)
            @param rel The relative path.
        */
        std::string AbsoluteResourcePath(std::string rel) {
            return JoinPaths({RESOURCE_PATH, rel});
        }
        /*
            Will take a relative path (so file.txt) and convert it to an absolute path to the save location based on the system (/.local/YourCompanyName/file.txt)
            @param rel The relative path.
        */
        std::string AbsoluteSavesPath(std::string rel) {
            return JoinPaths({SAVE_PATH, rel});
        }


        
        /*
            Attempts to check for an absolute path. Note that this will only work for checking against the SAVE_PATH and RESOURCE_PATH
            @param path The path to check.
        */
        bool IsAbsolutePath(const std::string& path) {
            if (path.empty()) return false;
            
            // Check if it starts with '/' on Unix or drive letter on Windows
            #ifdef _WIN32
                if (path.size() >= 2 && path[1] == ':') return true;
            #else
                if (path[0] == '/') return true;
            #endif
            
            // Also check if it matches our known paths
            if (!RESOURCE_PATH.empty() && path.compare(0, RESOURCE_PATH.size(), RESOURCE_PATH) == 0) return true;
            if (!SAVE_PATH.empty() && path.compare(0, SAVE_PATH.size(), SAVE_PATH) == 0) return true;
            
            return false;
        }
        /*
            Attempts to check if the file exists. Unless an absolute path from `AbsoluteSavesPath` is provided, it will automatically check in RESOURCE_PATH.
            @param path The path to check.
        */
       
        bool FileExists(std::string path) {
            if (!IsAbsolutePath(path)) {
                path = AbsoluteResourcePath(path);
            }

            SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "r");
            if (rw) {
                SDL_RWclose(rw);
                return true;
            }
            return false;
        }

        /*
            Attempts to read the file, returning text.
            Works seamlessly on desktop, Android, and Web.
        */
        std::string OpenFileAsText(std::string path) {
            if (!FileExists(path)) {
                throw std::runtime_error("File not found");
            }
            if (!IsAbsolutePath(path)) {
                path = AbsoluteResourcePath(path);
            }

            SDL_RWops* file = SDL_RWFromFile(path.c_str(), "r");
            if (!file) {
                throw std::runtime_error("Unable to read file!");
            }

            std::string text;
            char buffer[256];
            size_t bytesRead = 0;
            do {
                bytesRead = SDL_RWread(file, buffer, 1, sizeof(buffer) - 1);
                buffer[bytesRead] = '\0';
                text += buffer;
            } while (bytesRead > 0);

            SDL_RWclose(file);
            return text;
        }

        /*
            Reads a binary file and returns its contents as a string.
            Fully cross-platform — handles Android assets and preloaded web files.
        */
        std::string OpenFileAsBinary(std::string path) {
            if (!FileExists(path)) {
                throw std::runtime_error("File not found");
            }
            if (!IsAbsolutePath(path)) {
                path = AbsoluteResourcePath(path);
            }

            SDL_RWops* file = SDL_RWFromFile(path.c_str(), "rb");
            if (!file) {
                throw std::runtime_error("Unable to read file!");
            }

            Sint64 fileSize = SDL_RWsize(file);
            if (fileSize <= 0) {
                SDL_RWclose(file);
                throw std::runtime_error("File is empty or invalid!");
            }

            std::string data;
            data.resize(static_cast<size_t>(fileSize));

            size_t bytesRead = SDL_RWread(file, &data[0], 1, static_cast<size_t>(fileSize));
            SDL_RWclose(file);

            if (bytesRead != static_cast<size_t>(fileSize)) {
                throw std::runtime_error("Failed to read complete file!");
            }

            return data;
        }

        /*
            Writes text to a file, located at path.
            @param path If not absolute, will save to the SAVE_PATH.
        */
        void WriteFile(std::string path, const std::string& text) {
            if (!IsAbsolutePath(path)) {
                path = AbsoluteSavesPath(path);
            }

            SDL_RWops* file = SDL_RWFromFile(path.c_str(), "w");
            if (!file) {
                throw std::runtime_error("Unable to open file for writing: " + path);
            }

            size_t written = SDL_RWwrite(file, text.c_str(), 1, text.size());
            SDL_RWclose(file);

            if (written != text.size()) {
                throw std::runtime_error("Failed to write complete file: " + path);
            }
        }
    }
}
