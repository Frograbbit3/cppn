#pragma once
#include "SDL2/SDL_filesystem.h"
#include <SDL2/SDL_stdinc.h>
#include <any>
#include <cstddef>
#include <fmt/core.h>
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
            if (path.compare(0, RESOURCE_PATH.size(), RESOURCE_PATH) == 0 ||
                path.compare(0, SAVE_PATH.size(), SAVE_PATH) == 0) {
                return true;
            }
            return false;
        }
        /*
            Attempts to check if the file exists. Unless an absolute path from `AbsoluteSavesPath` is provided, it will automatically check in RESOURCE_PATH.
            @param path The path to check.
        */
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

        
        /*
            Attempts to read the file, returning text. Unless an absolute path from `AbsoluteSavesPath` is provided, it will automatically check in RESOURCE_PATH.
            @param path The file location to read.
        */
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
        /*
            Compresses a string using zlib.

            @param str The string to compress
            @param compressionLevel Pass in a Z_XXX compression level.
        */
        std::string CompressString(const std::string& str, int compressionLevel = Z_BEST_COMPRESSION) {
            z_stream zs{};
            if (deflateInit(&zs, compressionLevel) != Z_OK)
                throw std::runtime_error("deflateInit failed");

            zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(str.data()));
            zs.avail_in = str.size();

            int ret;
            char outbuffer[32768];
            std::string outstring;

            // compress in chunks
            do {
                zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
                zs.avail_out = sizeof(outbuffer);

                ret = deflate(&zs, Z_FINISH);

                if (outstring.size() < zs.total_out)
                    outstring.append(outbuffer, zs.total_out - outstring.size());
            } while (ret == Z_OK);

            deflateEnd(&zs);

            if (ret != Z_STREAM_END)
                throw std::runtime_error("Compression failed: " + std::to_string(ret));

            return outstring;
        }

        /*
            Decompresses a string.
            @param string The string to decompress.
        */
        std::string DecompressString(const std::string& str) {
            z_stream zs{};
            if (inflateInit(&zs) != Z_OK)
                throw std::runtime_error("inflateInit failed");

            zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(str.data()));
            zs.avail_in = str.size();

            int ret;
            char outbuffer[32768];
            std::string outstring;

            do {
                zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
                zs.avail_out = sizeof(outbuffer);

                ret = inflate(&zs, 0);

                if (outstring.size() < zs.total_out)
                    outstring.append(outbuffer, zs.total_out - outstring.size());
            } while (ret == Z_OK);

            inflateEnd(&zs);

            if (ret != Z_STREAM_END)
                throw std::runtime_error("Decompression failed: " + std::to_string(ret));

            return outstring;
        }
        class SaveData {
            private:
                std::map<std::string, std::vector<std::pair<std::string, std::any>>> data;
                enum class SaveTypes {
                    // Basic primitives
                    BOOL=5,
                    STRING=1,
                    INT=2,
                    FLOAT=3,
                    DOUBLE=4,
                };
                std::vector<std::string> SplitByBrackets(const std::string& k) {
                    std::vector<std::string> brackets;
                    size_t startSpot = std::string::npos;

                    for (size_t i = 0; i < k.size(); ++i) {
                        if (k[i] == '[')
                            startSpot = i;
                        else if (k[i] == ']' && startSpot != std::string::npos) {
                            size_t len = i - startSpot + 1; // include closing bracket
                            brackets.emplace_back(k.substr(startSpot, len));
                            startSpot = std::string::npos;  // reset
                        }
                    }
                    return brackets;
                }

            public:
                SaveData() {

                }

                /*
                    Set a value within the save data.
                    @param category The major catagory to put the data in. Use this for majorly different objects, such as Player and Enemy.
                    @param key The subkey to put the data in. Use this for properties of an object, like health for player or position for an enemy.
                    @param value The value of the data. The **only types** supported as of now are bools, std::string, ints, floats, and doubles. Passing in other values may cause it to fail to save.
                */
                template <typename T>
                void set(const std::string& category, const std::string& key, const T& value) {
                    auto& entries = data[category];
                    for (auto& [k, v] : entries) {
                        if (k == key) {
                            v = value;
                            return;
                        }
                    }
                    entries.emplace_back(key, value);
                }


                /*
                    Gets data from the save. You must perform an explicit cast to get the correct type of data back. (So use <int> if your data was saved as an int)
                    @param category The data's catagory.
                    @param key The data's subkey.
                    @param defaultValue Used if you want to set a default value, in case that key does not exist.
                */
                template <typename T>
                T get(const std::string& category, const std::string& key, const T& defaultValue = T{}) const {
                    auto it = data.find(category);
                    if (it == data.end()) return defaultValue;

                    for (const auto& [k, v] : it->second) {
                        if (k == key) {
                            if (v.type() == typeid(T))
                                return std::any_cast<T>(v);
                            else
                                throw std::bad_any_cast();
                        }
                    }
                    return defaultValue;
                }


                void load(std::string save) {

                    //checking if the save file is compressed or corrupted
                    if (save.substr(0,8) != "[CPPNSD]") {
                        save=DecompressString(save);
                        if (save.substr(0,7) != "[CPPNSD]") { //100% corrupted or invalid
                            throw std::runtime_error("Unable to load save, are you sure it's valid?");
                        }
                    }
                    std::vector<std::string> brack = SplitByBrackets(save);
                    std::string currentCatagory;
                    std::string currentKey;
                    int type;
                    int size;
                    std::string slice;
                    for (auto &k : brack) {
                        if (k[1] == '!') {
                            //catagory
                            currentCatagory = k.substr(2,k.size()-3);
                            
                        }else if (k[1] == '@') {
                            currentKey= k.substr(2,k.size()-3);
                        }else if (k[1] == '%') {
                            type = std::stoi(k.substr(2, 1));
                            if (data.find(currentCatagory) == data.end()) {
                                data[currentCatagory] = {};
                            }
                            //[%20000000250]
                           // std::cout << k << std::endl;
                            size = std::stoi(k.substr(3, 8));
                            slice = k.substr(11, size) ;
                            switch (type){
                                case static_cast<int>(SaveTypes::STRING):
                                    data[currentCatagory].emplace_back(currentKey, std::any(slice));
                                    break;
                                case static_cast<int>(SaveTypes::BOOL):
                                    data[currentCatagory].emplace_back(currentKey, std::any((slice == "1") ? true : false));
                                    break;
                                case static_cast<int>(SaveTypes::INT):
                                    data[currentCatagory].emplace_back(currentKey, std::any(std::stoi(slice)));
                                    break;
                                case static_cast<int>(SaveTypes::DOUBLE):
                                    data[currentCatagory].emplace_back(currentKey, std::any(std::stod(slice)));
                                    break;
                                case static_cast<int>(SaveTypes::FLOAT):
                                    data[currentCatagory].emplace_back(currentKey, std::any(std::stof(slice)));
                                    break;

                            }
                        }
                    }
                }
                /*
                    Saves the data and returns a string. 

                    @param compress By default true but will perfrom strong zlib compression.
                    @note WriteSaveFile() is recommended over this and manual saving. 
                */
                std::string save(bool compress=false) {
                    std::string dt;
                    dt.append(fmt::format("[CPPNSD]"));
                    for (const auto& m : data) {
                        //this will be a key and a vector 
                        dt.append(fmt::format("[!{}]", m.first));
                       for (const auto& obj : m.second) {
                            dt.append(fmt::format("[@{}]", obj.first));

                            if (obj.second.type() == typeid(bool)) {
                                bool val = std::any_cast<bool>(obj.second);
                                std::string strVal = val ? "1" : "0";
                                dt.append(fmt::format("[%{}{:08}{}]",
                                    static_cast<int>(SaveTypes::BOOL),
                                    strVal.size(),
                                    strVal));
                            }
                            else if (obj.second.type() == typeid(std::string)) {
                                std::string val = std::any_cast<std::string>(obj.second);
                                dt.append(fmt::format("[%{}{:08}{}]",
                                    static_cast<int>(SaveTypes::STRING),
                                    val.size(),
                                    val));
                            }
                            else if (obj.second.type() == typeid(int)) {
                                std::string val = fmt::format("{}", std::any_cast<int>(obj.second));
                                dt.append(fmt::format("[%{}{:08}{}]",
                                    static_cast<int>(SaveTypes::INT),
                                    val.size(),
                                    val));
                            }
                            else if (obj.second.type() == typeid(float)) {
                                std::string val = fmt::format("{:.6f}", std::any_cast<float>(obj.second));
                                dt.append(fmt::format("[%{}{:08}{}]",
                                    static_cast<int>(SaveTypes::FLOAT),
                                    val.size(),
                                    val));
                            }
                            else if (obj.second.type() == typeid(double)) {
                                std::string val = fmt::format("{:.6f}", std::any_cast<double>(obj.second));
                                dt.append(fmt::format("[%{}{:08}{}]",
                                    static_cast<int>(SaveTypes::DOUBLE),
                                    val.size(),
                                    val));
                            }
                            else {
                                dt.append(fmt::format("[%??{:08}UNKNOWN({})]",
                                    0, obj.second.type().name()));
                            }
                        }


                    }
                    if (compress) {
                        return CompressString(dt);
                    }
                    return dt;
                }


        };
        
        /*
            Writes text to a file, located at path.
            @param path If not absolute, will save to the SAVE_PATH.
        */
        void WriteFile(std::string path, std::string text) {
            if (!IsAbsolutePath(path)) {
                path=AbsoluteSavesPath(path);
            }
            FILE *file = fopen(path.c_str(), "w");
            fprintf(file, "%s", text.c_str());
            fclose(file);
            return;
        }

        /*
            The preferred way to save a save file.
            @param path The file to save to. This will be converted to absolute and saved to SAVE_PATH if another absolute path is not provided.
        */
        void WriteSaveFile(std::string path, SaveData &save) {
            WriteFile(path, save.save().c_str());
            return;
        }

        /*
            This is the way to load a save file.

            @param path The path to read the save from. Will be converted to absolute based on SAVE_PATH if an absolute path is not provided.
            @return SaveData ready for you to use. You can use .save on this if you want to load it into your own SaveData object.
        */
        SaveData OpenSaveFile(std::string path) {
            if (!FileExists(path)) {
                throw std::runtime_error("File not found");
            }
            if (!IsAbsolutePath(path)) {
                path=AbsoluteSavesPath(path);
            }
            SaveData save;
            save.load(OpenFileAsText(path));
            return save;
        }

        

    }
}
