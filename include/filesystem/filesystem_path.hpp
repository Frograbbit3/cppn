#pragma once
#include <string>
#include "filesystem_core.hpp"
#include "utils/string_utils.hpp"


using namespace CPPN::FileSystem;
namespace CPPN {
    namespace FileSystem {
        struct Path
        {
            //The path provided.
            std::string rel_path;
            //The absolute path
            std::string value;
            //If the file has been found. Running .update on this will recheck.
            bool exists=true;
                        //Creates a new Path object.
            Path(const std::string& path): rel_path(path) {
                rel_path = std::string(path);
                if (!IsAbsolutePath(rel_path)) {
                    value = AbsoluteResourcePath(rel_path);
                    if (!FileExists(value)) {
                        value = AbsoluteSavesPath(rel_path);
                        if (!FileExists(value)) {
                            value = rel_path;
                            exists=false;
                        }
                    }
                }
            }
            /*
                Opens a file as a text format. 
                @return The contents of the file as a string, or null.
            */
            std::string open() {
                return OpenFileAsText(this->value);
            }

            /*
                Reads a binary file and returns its contents as a string.
                @return A string with the bytes in it.
            */
            std::string binary() {
                return OpenFileAsBinary(this->value);
            }

            /*
                Writes an std::string to the file.
            */
            void write(const std::string& data) {
                WriteFile(this->value, data);
            }
        };
        
    }
}