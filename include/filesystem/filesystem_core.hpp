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
                std::vector<std::string> SplitByBrackets(std::string k) {
                    std::vector<std::string> brackets;

                    int startSpot = 0;
                    int i = 0;
                    //k.erase(std::remove(k.begin(), k.end(), '\n'), k.end());
                    //k.erase(std::remove(k.begin(), k.end(), '\r'), k.end());
                    for (const char letter : k) {
                        if (letter == '[') {
                            startSpot=i;
                        }
                        if (letter == ']') {
                            //std::cout << "identified: " << k.substr(startSpot, i+1) << std::endl;
                            brackets.emplace_back(k.substr(startSpot, i+1));                            
                        }
                        i++;
                    }
                    return brackets;
                }
            public:
                SaveData() {

                }
                void add(std::string catagory, std::string key, std::any value) {
                    if (data.find(catagory)== data.end()) {
                        data[catagory] = {};
                    }
                    data[catagory].emplace_back(key, value);
                    return;
                }

                std::any get(std::string catagory, std::string key) {
                    for (auto &v : data[catagory]) {
                        if (v.first == key) {
                            return v.second;
                        }
                    }
                    return NULL;
                }

                void load(std::string save) {
                    std::vector<std::string> brack = SplitByBrackets(save);
                    std::string currentCatagory;
                    std::string currentKey;
                    int type;
                    int size;
                    std::string slice;
                    for (auto &k : brack) {
                        if (k[1] == '!') {
                            //catagory
                            currentCatagory = k.substr(2,k.size()-1);
                        }else if (k[1] == '@') {
                            currentKey= k.substr(2,k.size()-1);
                        }else if (k[1] == '%') {
                            type = k[2];
                            if (data.find(currentCatagory) == data.end()) {
                                data[currentCatagory] = {};
                            }
                            //[%20000000250]
                           // std::cout << k << std::endl;
                            size = std::stoi(k.substr(3, 8));
                         //   std::cout << "size: " << size<<std::endl;

                            slice = k.substr(11, size) ;
                       //     std::cout << "sliced: " << slice << std::endl;
                            switch (type){
                                case static_cast<int>(SaveTypes::STRING):
                                    data[currentCatagory].emplace_back(currentKey, slice);
                                case static_cast<int>(SaveTypes::BOOL):
                                    data[currentCatagory].emplace_back(currentKey, (slice == "1") ? true : false);
                                case static_cast<int>(SaveTypes::INT):
                                    data[currentCatagory].emplace_back(currentKey, std::stoi(slice));
                                case static_cast<int>(SaveTypes::DOUBLE):
                                    data[currentCatagory].emplace_back(currentKey, std::stod(slice));

                            }
                        }
                    }
                }
                std::string save() {
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
                                dt.append(fmt::format("[%{}{:08}{}",
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
                  //  std::cout << dt << std::endl;
                    return dt;
                }


        };
    }
}
