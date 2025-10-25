#include "SDL2/SDL.h"
#pragma once
namespace CPPN {
    namespace Graphics {
        SDL_Window* window;
        SDL_Renderer* render;
        /*
            Sets up the window side of things. Creates a window.
        */
        void InitGraphics
           window = SDL_CreateWindow("Example Window");
       }
    }
}