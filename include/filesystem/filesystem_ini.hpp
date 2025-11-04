#pragma once
#include "filesystem_path.hpp"
#include <unordered_map>
#include <sstream>
#include "utils/string_utils.hpp"
using namespace CPPN::Utils::StringUtils;
namespace CPPN {
    namespace FileSystem {
        class IniParser {
            private:
                Path location;
                std::string contents;
                std::unordered_map<std::string,std::vector<std::pair<std::string, std::string>>> data;
                std::string catagory;
                std::string key;
                bool loaded = false;
                void load_ini() {
                    std::istringstream stream(contents);
                    std::string line;

                    while (std::getline(stream, line)) {
                        if (line[0] == ';') {
                            continue;
                        }
                        if (strip(line) == "") {
                            continue;
                        }
                        line=lstrip(line);
                        if (line[0] == '[') {
                            size_t end = line.find(']');
                            if (end != std::string::npos) {
                                catagory = line.substr(1, end - 1);
                            }
                        }else{
                            //assume key pair
                            size_t end = line.find('=');
                            if (end != std::string::npos) {
                                key = strip(line.substr(0, end-1));
                                if (data.find(catagory) == data.end()) {
                                    data[catagory] = {};
                                }
                                data[catagory].emplace_back(key, lstrip(line.substr(end+1, std::string::npos)));
                            }

                        }
                    }
                    for (const auto& [outerKey, innerMap] : data) {
                        std::cout << "catagory: " << outerKey << "\n";
                        for (const auto& d : innerMap) {
                            std::cout << "key: " << d.first << ", value: " << d.second << std::endl;
                        }
                    }
                }
            public:
                IniParser(std::string path) : location(path) {}
                void load() {
                    if (!location.exists) {throw std::runtime_error("Please do not call .load on an uncreated file! use .set instead.");}
                    contents = location.open();
                    load_ini();
                    loaded = true;
                }

        };
    }
}