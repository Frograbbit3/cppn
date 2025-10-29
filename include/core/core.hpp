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

        /*
            The main init function. Use this to create the window.
            @param width The width of the window.
            @param height The height of the window.
            @param title The title of the window.

            @note These params are just passed into CPPN::Graphics::CreateWindow().
            
        */
        void Init(int width, int height, std::string title)
        {
            CPPN::Graphics::CreateWindow(width, height, title);
            init = true;
        }
        /* 
        The main tick function. If you don't want to use CPPN::Core::Run(), you may call this in your own loop.
        
        @note By running it on its own you will most likely break emscripten which crashes if it doesn't manage the loop.

        @return A bool if to keep running.
        */
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
            if (CPPN::Input::keysPressed.size() > 0) {
                CallMacro(CPPN::Enums::Event::ON_KEY_HOLD);
            }

#ifndef __EMSCRIPTEN__
            SDL_Delay(16); // only delay in native builds
#endif
            return true;
        }

#ifdef __EMSCRIPTEN__
        /// Wrapper to satisfy emscripten_set_main_loop’s void() signature
        void TickWrapper() { Tick(); }
#endif
        /*Call this to start the main loop.*/
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
