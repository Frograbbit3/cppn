#pragma once
#include "SDL2/SDL.h"
#include <iostream>
#include <vector>
#include "graphics_general.hpp"
namespace CPPN {
    namespace Graphics {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Event e;
        std::vector<CPPN::Graphics::BaseShape*> shapes;
        bool running = true;
        bool WindowInit = false;
        void CreateWindow(int width, int height, std::string title) {
            if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                std::cout << "Error initing SDL" << std::endl;
            }
            // create a normal shown window (not Vulkan) so the 2D renderer works
            window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
            if (window == NULL) {
                std::cout << "Error creating SDL window" << std::endl;
            }
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == NULL) {
                std::cout << "Error creating SDL renderer" << std::endl;
            } else {
                // enable alpha blending for shapes with transparency
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            }
            WindowInit = true;
        }
        void CloseWindow() {
            if (WindowInit) {
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();
            }
        }
        void AddShape(CPPN::Graphics::BaseShape* shape) {
            shapes.emplace_back(shape);
        }
        void DrawShapes() {
            // clear to black
            if (renderer) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
            }

            for (CPPN::Graphics::BaseShape* shape : shapes) {
                if (!shape) continue;
                // set draw color from the shape's color (use components, SDL expects 4 uint8_t args)
                auto col = shape->getColor();
                SDL_SetRenderDrawColor(renderer, col.red, col.green, col.blue, col.alpha);
                
                shape->update();
                if (renderer) shape->draw(renderer);
            }

            if (renderer) SDL_RenderPresent(renderer);
        }
        bool TickWindow() {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                }
            }
        
            DrawShapes();
            return running;
        }

    }
}