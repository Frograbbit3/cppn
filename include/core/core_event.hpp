#pragma once
#include "SDL2/SDL.h"
#include "core.hpp"
namespace CPPN {
    namespace Private {
        bool running=true;
        SDL_Event e;
        /*The main loop for processing events.
        
        @return A bool for if the program should stop.
        */
        bool EventLoop() {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    CPPN::Input::mouse1down = true;
                    CPPN::Core::CallMacro(CPPN::Enums::Event::ON_MOUSE_CLICK);
                }
                if (e.type == SDL_MOUSEBUTTONUP) {
                    CPPN::Input::mouse1down= false;
                    CPPN::Core::CallMacro(CPPN::Enums::Event::ON_MOUSE_RELEASE);
                }
                
                if (e.type == SDL_KEYDOWN) {
                    SDL_Keycode key = e.key.keysym.sym;
                    std::string keyName(SDL_GetKeyName(key));
                    CPPN::Input::KeyDown(keyName);
                    CPPN::Core::CallMacro(CPPN::Enums::Event::ON_KEY_PRESS);
                }
                if (e.type == SDL_KEYUP) {
                    SDL_Keycode key = e.key.keysym.sym;
                    std::string keyName(SDL_GetKeyName(key));
                    CPPN::Input::KeyUp(keyName);
                    CPPN::Core::CallMacro(CPPN::Enums::Event::ON_KEY_RELEASE);
                }
                if (e.type == SDL_MOUSEMOTION && CPPN::Input::mouse1down) {
                    CPPN::Input::MoveMouse(e.motion.x, e.motion.y);
                    CPPN::Core::CallMacro(CPPN::Enums::Event::ON_MOUSE_DRAG);
                }
                if (e.type == SDL_MOUSEMOTION) {
                    CPPN::Input::MoveMouse(e.motion.x, e.motion.y);
                    CPPN::Core::CallMacro(CPPN::Enums::Event::ON_MOUSE_MOVE);
                }
            }
            return running;
        }
    }
}