#pragma once
#include <string>
#include <type_traits>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <string_view>
#include "utils/string_utils.hpp"
namespace CPPN {
    namespace Utils {
        template <typename T>
        static T convert(const std::string& str) {
            if constexpr (std::is_same_v<T, std::string>) {
                return str;
            } else if constexpr (std::is_same_v<T, int>) {
                return std::stoi(str);
            } else if constexpr (std::is_same_v<T, double>) {
                return std::stod(str);
            } else if constexpr (std::is_same_v<T, bool>) {
                std::string lower = str;
                std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                return (lower == "true" || lower == "yes");
            } else {
                static_assert(!sizeof(T*), "Unsupported type for CPPN::Utils::convert");
            }
        }
        template <typename T>
        std::string to_string(const T& value) {
            if constexpr (std::is_same_v<T, std::string>) {
                return value;
            }
            else if constexpr (std::is_same_v<T, const char*>) {
                return std::string(value);
            }
            else if constexpr (std::is_same_v<T, bool>) {
                return value ? "true" : "false";
            }
            else if constexpr (std::is_arithmetic_v<T>) {
                return std::to_string(value);
            }
            else if constexpr (std::is_enum_v<T>) {
                using Under = typename std::underlying_type<T>::type;
                return std::to_string(static_cast<Under>(value));
            }
            else if constexpr (std::is_same_v<T, char*>) {
                return std::string(value);
            }
            else {
                static_assert(!sizeof(T*), "Unsupported type for to_string()");
            }
        }
        // Overload for string literals and char arrays
        template <size_t N>
        inline std::string to_string(const char (&value)[N]) {
            return std::string(value);
        }
        inline std::string to_string(std::string_view sv) {
            return std::string(sv);
        }
         inline bool is_int(const std::string& s) {
        if (s.empty()) return false;
        size_t i = 0;
        if (s[0] == '-' || s[0] == '+')
            i++;
        bool has_digit = false;
        for (; i < s.size(); ++i) {
            if (!std::isdigit(static_cast<unsigned char>(s[i])))
                return false;
            has_digit = true;
        }
        return has_digit;
    }

    inline bool is_double(const std::string& s) {
        if (s.empty()) return false;
        char* end = nullptr;
        std::strtod(s.c_str(), &end);
        // Must have parsed *something* and consumed entire string
        return (end != s.c_str() && *end == '\0');
    }

    inline bool is_bool(const std::string& s) {
        if (s.empty()) return false;
        std::string lower = s;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return (lower == "true" || lower == "false" ||
                lower == "yes"  || lower == "no" ||
                lower == "on"   || lower == "off" ||
                lower == "1"    || lower == "0");
        }
    }
}