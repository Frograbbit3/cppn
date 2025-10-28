#pragma once
#include <iostream>
#include "../graphics/graphics_core.hpp"
#include "macros.hpp"
#include <map>
#include <functional>
#include <any>

namespace CPPN {
    namespace Core {
        // Store macros as callable objects with a unified void() signature.
        bool paused = false;
        bool init=false;

        void Init(int width, int height, std::string title) {
            CPPN::Graphics::CreateWindow(width, height, title);
            init=true;
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