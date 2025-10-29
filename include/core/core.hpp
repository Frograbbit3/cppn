#pragma once
#include <iostream>
#include "../graphics/graphics_core.hpp"
#include "macros.hpp"
#include "core_event.hpp"
#include <map>
#include <functional>
#include <any>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace CPPN
{
    namespace Core
    {

        bool paused = false;
        bool init = false;

        void Init(int width, int height, std::string title)
        {
            CPPN::Graphics::CreateWindow(width, height, title);
            init = true;
        }

        bool Tick()
        {
            CPPN::Graphics::TickWindow();
            if (CPPN::Private::EventLoop() == false)
            {
#ifdef __EMSCRIPTEN__
                emscripten_cancel_main_loop();
#else
                return false;
#endif
            }

            if (!paused)
            {
                CallMacro(CPPN::Enums::Event::ON_TICK);
            }

#ifndef __EMSCRIPTEN__
            SDL_Delay(16); // only delay in native builds
#endif
            return true;
        }

#ifdef __EMSCRIPTEN__
        // Wrapper to satisfy emscripten_set_main_loop’s void() signature
        void TickWrapper() { Tick(); }
#endif

        void Run()
        {
#ifdef __EMSCRIPTEN__
            emscripten_set_main_loop(TickWrapper, 0, 1);
#else
            while (Tick())
            { /* keep going until TickWindow() returns false */
            }
#endif
        }

    } // namespace Core
} // namespace CPPN
