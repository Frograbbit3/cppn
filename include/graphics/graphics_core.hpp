#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include "../core/input_core.hpp"
#include "../core/macros.hpp"
#include "../core/core.hpp"
#include "../shapedesigner/core.hpp"
#include "../widgets/widgets_core.hpp"
namespace CPPN {
    namespace Graphics {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        std::vector<CPPN::ShapeDesigner::Shape*> shapes;
        bool running = true;
        bool WindowInit = false;
        // Variables to track dragging state
        int dragOffsetX = 0;
        int dragOffsetY = 0;


        /*
            Creates a window. 

            @param width The width of the window.
            @param height The height of the window.
            @param title The title of the window.
            
            @note Use CPPN::Core::Init(width, height, title) over this.
        */
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
            int imgFlags = IMG_INIT_PNG;
            if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
                std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
            }
        }

        /*
            Closes the window.
        */
        void CloseWindow() {
            if (WindowInit) {
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                // quit SDL_image first
                IMG_Quit();
                SDL_Quit();
            }
        }

        /*
            @private
            Loads an image into an SDL_Texture. Designed for private use, but can return an SDL_Texture from a file.

            @note Unlike the filesystem, this will not automatically get the resources directory. For consistent use across platforms, use the FileSystem's AbsoluteSavesPath().
        */
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

        /*
            Adds a shape to the internal shapes vector. Private use only.
            
            @note BaseShapes automatically add themselves to this vector.
        */
        void AddShape(CPPN::ShapeDesigner::Shape* shape) {
            shapes.emplace_back(shape);
        }


        /*
            Internal function to process dragging and draw the shapes to the screen. Private use, but callable manually.

            @note No guarantees this will keep working if you call it manually. CPPN::Core::Run() will handle this.
        */
        void DrawShapes() {
            // clear to black
            if (renderer) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
            }

            for (CPPN::ShapeDesigner::Shape* shape : shapes) {
                shape->cache();
                // draw full texture (srcrect=nullptr) at cached destination rect
                SDL_RenderCopyEx(renderer, shape->cached, nullptr, shape->cached_rect, shape->transforms.rotation, NULL, SDL_FLIP_NONE);
            } 

            if (renderer) SDL_RenderPresent(renderer);
        }
        /*
            Private use only. Update function for drawing shapes. Called by CPPN::Core::Run()
        */
        void TickWindow() {
            DrawShapes();
        }

    }
}