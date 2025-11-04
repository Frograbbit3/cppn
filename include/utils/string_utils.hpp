#pragma once
#include <string>
#include <algorithm>

namespace CPPN
{
    namespace Utils {
        namespace StringUtils {
            // Removes all of a character at the end of a string. Defaults to space.
            std::string rstrip(const std::string& str, const char ch = ' ') {
                size_t end = str.find_last_not_of(ch);
                if (end != std::string::npos) {
                    return str.substr(0, end + 1);
                }
                return "";
            }

            // Removes all of a character at the start of a string. Defaults to space.
            std::string lstrip(const std::string& str, const char ch = ' ') {
                size_t start = str.find_first_not_of(ch);
                if (start != std::string::npos) {
                    return str.substr(start);
                }
                return "";
            }

            // Removes all spaces, tabs, and newlines.
            std::string strip(const std::string& str) {
                std::string result;
                std::copy_if(str.begin(), str.end(), std::back_inserter(result),
                    [](unsigned char c) { return c != ' ' && c != '\t' && c != '\n'; });
                return result;
            }
        }
    }
} // namespace CPPN
