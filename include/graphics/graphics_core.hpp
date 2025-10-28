#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include "graphics_general.hpp"
#include "../core/input_core.hpp"
#include "../core/macros.hpp"
#include "../core/core.hpp"
namespace CPPN {
    namespace Graphics {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Event e;
        std::vector<CPPN::Graphics::BaseShape*> shapes;
        bool running = true;
        bool WindowInit = false;
        // Variables to track dragging state
        BaseShape* draggedShape = nullptr;
        int dragOffsetX = 0;
        int dragOffsetY = 0;

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

            // initialize SDL_image for common formats (PNG, JPG)
            int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
            if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
                std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
            }
        }
        void CloseWindow() {
            if (WindowInit) {
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                // quit SDL_image first
                IMG_Quit();
                SDL_Quit();
            }
        }

        SDL_Texture* LoadImage(const std::string &img) {
            if (!WindowInit || renderer==nullptr) {
                std::cerr << "LoadImage: Renderer/window not initialized" << std::endl;
                return nullptr;
            }
            SDL_Texture* tex = IMG_LoadTexture(renderer, img.c_str());
            if (!tex) {
                std::cerr << "IMG_LoadTexture failed for '" << img << "': " << IMG_GetError() << std::endl;
            }
            return tex;
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

                // Process dragging
                if (CPPN::Input::leftMouseDown) {
                    if (!draggedShape && shape->draggable && shape->isColliding(CPPN::Input::mouseX, CPPN::Input::mouseY)) {
                        draggedShape = shape;
                        dragOffsetX = CPPN::Input::mouseX - shape->x;
                        dragOffsetY = CPPN::Input::mouseY - shape->y;
                    }
                    if (draggedShape == shape) {
                        shape->setPosition(CPPN::Input::mouseX - dragOffsetX, CPPN::Input::mouseY - dragOffsetY);
                    }
                } else {
                    draggedShape = nullptr; // Reset dragging state when the mouse button is released
                }

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
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    CPPN::Input::leftMouseDown = true;
                }
                if (e.type == SDL_MOUSEBUTTONUP) {
                    CPPN::Input::leftMouseDown = false;
                }
                if (e.type == SDL_MOUSEMOTION && CPPN::Input::leftMouseDown) {
                    CPPN::Input::MoveMouse(e.motion.x, e.motion.y);
                    CPPN::Core::CallMacro("mousedrag");
                }
                if (e.type == SDL_MOUSEMOTION) {
                    CPPN::Input::MoveMouse(e.motion.x, e.motion.y);
                    CPPN::Core::CallMacro("mousemove");
                }
            }
        
            DrawShapes();
            return running;
        }

    }
}