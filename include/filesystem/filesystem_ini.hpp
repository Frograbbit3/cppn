#pragma once
#include "filesystem_path.hpp"
#include <map>
#include <sstream>
#include "utils/utils.h"
using namespace CPPN::Utils::StringUtils;
using namespace CPPN::Utils;
namespace CPPN {
    namespace FileSystem {
        class IniParser {
            private:
                Path location;
                std::string contents;
                std::map<std::string,std::vector<std::pair<std::string, std::string>>> data;
                std::string catagory;
                std::string key;
                size_t end;
                std::string trimmed;
                bool loaded = false;
                void load_ini() {
                    std::istringstream stream(contents);
                    std::string line;
                    while (std::getline(stream, line)) {
                        switch (line[0]){
                            case ';':
                                continue;
                            case '#':
                                continue;
                            case '[':
                                line=lstrip(line);
                                end = line.find(']');
                                if (end != std::string::npos) {
                                    if (end+1 < strip(line).size()) {
                                        catagory = "";
                                        continue;
                                    }
                                    
                                    catagory = strip(line.substr(1, end-1));
                                }
                                break;
                            default:
                                if (catagory == "") {continue;}
                                end = line.find('=');
                                if (end == std::string::npos) continue;
                                
                                key = strip(line.substr(0, end));
                                if (key == "") {continue;}
                                trimmed = strip(line.substr(end+1));
                                
                                if (!trimmed.empty() && (trimmed[0] == '"' || trimmed[0] == '\'')){
                                    if (trimmed.size() >= 2 && (trimmed.back() == '"' || trimmed.back() == '\'')){
                                        trimmed = trimmed.substr(1, trimmed.size()-2);
                                    }
                                }
                                
                                data[catagory].emplace_back(key, trimmed);
                                break;                    
                            
                        }
                    }
                }
                std::string getSave() {
                    std::string newData="";
                    #ifndef NO_WATERMARK
                        newData += "; Created by CPPN\n";
                    #endif
                    bool needQuote = false;
                    for (const auto &[category, d] : data) {
                        newData += fmt::format("\n[{}]\n", category); 
                        for (const auto &[key, val]: d) {
                            if (!is_int(val) && !is_bool(val) && !is_double(val)) {
                                newData += fmt::format("{} = \"{}\"", key, val);
                            }else{
                                newData += fmt::format("{} = {}", key, val);
                            }
                            newData += "\n";
                        }
                    }
                    return newData;
                }
            public:
                IniParser(std::string path) : location(path) {}
                        
                void load() {
                    if (!location.exists) {return;}
                    contents = location.open();
                    load_ini();
                    loaded = true;
                }

                bool exists(const std::string& category, const std::string &key) {
                    if (data.find(category) != data.end()) {
                        for (std::pair<std::string, std::string> &value : data[category]) {
                            if (value.first == key) {
                                return true;
                            }
                        }
                    }
                    return false;
                }
                template <typename T>
                T get(const std::string& category, const std::string& key) {
                    if (data.find(category) != data.end()) {
                        for (std::pair<std::string, std::string> &value : data[category]) {
                            if (value.first == key) {
                                return convert<T>(value.second);
                            }
                        }
                    }
                    throw std::runtime_error("Category is not found!");
                }

                template <typename T>
                void set(const std::string& category, const std::string& key, const T& value) {
                    if (data.find(category) == data.end()) {data[category] = {};}
                    for (std::pair<std::string, std::string> &m : data[category]) {
                        if (m.first == key) {
                            m.second = to_string(value);
                            return;
                        }
                    }
                    data[category].emplace_back(key, to_string(value));
                    return;
                }

                void save(const std::string& fileName="") {
                    if (fileName == "") {
                        location.write(getSave());
                    }else{
                        CPPN::FileSystem::WriteFile(fileName, getSave());
                    }
                }



        };
    }
}