#include <iostream>
#include "graphics.hpp"
#include <map>
#include <functional>
#include <any>
#pragma once
namespace CPPN {
    namespace Core {
        // Store macros as callable objects with a unified void() signature for now.
        // This keeps the API simple and fixes the compilation error when calling
        // macros with no arguments (e.g. "tick"). If you need variadic/typed
        // macros later, we can implement a type-erased wrapper.
        std::map<std::string, std::function<void()>> MACROS;
        bool paused = false;

        void Init(int width, int height, std::string title) {
            CPPN::Graphics::CreateWindow(width, height, title);
        }

        // Store any callable that is convertible to std::function<void()>
        template<typename Fn>
        void AssignMacro(std::string macroName, Fn func) {
            if (MACROS.contains(macroName)){
                return;
            }
            MACROS[macroName] = std::function<void()>(std::move(func));
        }

        // Call a stored macro with no arguments.
        void CallMacro(std::string macroName){
            auto it = MACROS.find(macroName);
            if (it == MACROS.end()) {
                return;
            }
            auto &fn = it->second;
            if (fn) {
                fn();
            } else {
                throw std::runtime_error("Macro is empty or cannot be called with no args");
            }
        }
            void Run() {
                while (true) {
                    if (CPPN::Graphics::TickWindow() == false){
                        break;
                    }
                    if (!paused) {
                        CallMacro("tick");
                    }
                    SDL_Delay(16);
                }
            }


    }
}