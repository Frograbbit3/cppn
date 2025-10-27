#pragma once
#include "SDL2/SDL.h"
namespace CPPN {
    namespace Graphics {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Event e;
        bool running = true;
        bool WindowInit = false;
        void CreateWindow(int width, int height, std::string title) {
            if (SDL_Init(SDL_INIT_VIDEO) != 0) {std::cout<<"Error initing SDL"<<std::endl;}
            window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_VULKAN);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (window == NULL) {std::cout<<"Error creating SDL window"<<std::endl;}
            SDL_Delay(3000);
            WindowInit=true;
        }
        void CloseWindow() {
            if (WindowInit) {
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
            }
        }
        bool TickWindow() {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                }
            }
            return running;
        }

    }
}