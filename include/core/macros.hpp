#pragma once
#include <string>
#include <map>
#include <functional>
#include <stdexcept>
namespace CPPN {
    namespace Core {
        std::map<std::string, std::function<void()>> MACROS;
        // Assign a macro with any callable that is convertible to std::function<void()>.
        template<typename Fn>
        void AssignMacro(std::string macroName, Fn func) {
            if (MACROS.contains(macroName)) {
                return;
            }
            MACROS[macroName] = std::function<void()>(std::move(func));
        }

        // Call a stored macro with no arguments.
        void CallMacro(std::string macroName) {
            auto it = MACROS.find(macroName);
            if (it == MACROS.end()) {
                return;
            }
            auto &fn = it->second;
            if (fn) {
                fn();
            } else {
                throw std::runtime_error("Macro is empty or cannot be called");
            }
        }

    }
}