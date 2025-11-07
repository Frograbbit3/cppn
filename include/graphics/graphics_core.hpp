#pragma once
#include "backend/backend.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "../core/input_core.hpp"
#include "../core/macros.hpp"
#include "../core/core.hpp"
#include "../shapedesigner/core.hpp"
namespace CPPN {
    namespace Graphics {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        std::vector<CPPN::ShapeDesigner::Shape*> shapes;
        bool running = true;
        bool WindowInit = false;
        CPPN::Graphics::Color background {255,255,255,255};
        // Variables to track dragging state
        CPPN::ShapeDesigner::Shape* draggingShape = nullptr;
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
            SDL_Init(SDL_INIT_AUDIO);
            // initialize SDL_image for common formats (PNG, JPG)
        }

        /*
            Closes the window.
        */
        void CloseWindow() {
            if (WindowInit) {
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                // quit SDL_image first
                // quit SDL_ttf
                SDL_Quit();
            }
        }

        /*
            Adds a shape to the internal shapes vector. Private use only.
            
            @note BaseShapes automatically add themselves to this vector.
        */
        void AddShape(CPPN::ShapeDesigner::Shape* shape) {
            shapes.emplace_back(shape);
        }

        // Remove a shape pointer if present; safe to call multiple times
        void RemoveShape(CPPN::ShapeDesigner::Shape* shape) {
            auto it = std::remove(shapes.begin(), shapes.end(), shape);
            if (it != shapes.end()) {
                shapes.erase(it, shapes.end());
            }
        }


        /*
            Internal function to process dragging and draw the shapes to the screen. Private use, but callable manually.

            @note No guarantees this will keep working if you call it manually. CPPN::Core::Run() will handle this.
        */
        void DrawShapes() {
            // clear to background color
            if (renderer) {
                SDL_SetRenderDrawColor(renderer, background.red, background.green, background.blue, background.alpha);
                SDL_RenderClear(renderer);
            }

            for (CPPN::ShapeDesigner::Shape* shape : shapes) {
                // ensure texture and destination rect are up to date
                shape->cache();

                #ifdef SHOW_HITBOXES
                    SDL_Rect rect;
                    rect.x = shape->cached_rect->x;
                    rect.y = shape->cached_rect->y;
                    rect.w = shape->cached_rect->w;
                    rect.h = shape->cached_rect->h;
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128);
                    SDL_RenderFillRect(renderer, &rect);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                #endif

                // process click/drag
                if (Input::mouse1down) {
                    if (!draggingShape && shape->IsColliding(Input::mouseX, Input::mouseY)) {
                        draggingShape = shape;
                        dragOffsetX = shape->position.x - Input::mouseX;
                        dragOffsetY = shape->position.y - Input::mouseY;
                        if (shape->OnClick) {
                            shape->OnClick(dragOffsetX, dragOffsetY);
                        }
                    }
                    if (draggingShape == shape && shape->draggable) {
                        shape->position.x = Input::mouseX + dragOffsetX;
                        shape->position.y = Input::mouseY + dragOffsetY;
                        shape->cache();
                    }
                } else {
                    if (draggingShape) {
                        if (draggingShape->OnRelease) {
                            draggingShape->OnRelease(dragOffsetX, dragOffsetY);
                        }
                        draggingShape = nullptr;
                    }
                }

                // draw full texture (srcrect=nullptr) at cached destination rect
                // Compose simple parent->child transforms for rotation around parent center if a parent exists.
                SDL_Rect dst = *shape->cached_rect; // start from cached absolute rect (translation applied in cache)

                double angle = static_cast<double>(shape->transforms.rotation);
                if (shape->parent) {
                    const double parentCx = shape->parent->position.x + shape->parent->size.width * 0.5;
                    const double parentCy = shape->parent->position.y + shape->parent->size.height * 0.5;
                    const double childLocalCx = shape->position.x + shape->size.width * 0.5;
                    const double childLocalCy = shape->position.y + shape->size.height * 0.5;
                    double vx = childLocalCx - shape->parent->size.width * 0.5;
                    double vy = childLocalCy - shape->parent->size.height * 0.5;
                    const double rad = shape->parent->transforms.rotation * M_PI / 180.0;
                    const double cs = std::cos(rad);
                    const double sn = std::sin(rad);
                    const double rx = cs * vx - sn * vy;
                    const double ry = sn * vx + cs * vy;
                    const double worldCx = parentCx + rx;
                    const double worldCy = parentCy + ry;
                    dst.x = static_cast<int>(std::round(worldCx - dst.w * 0.5));
                    dst.y = static_cast<int>(std::round(worldCy - dst.h * 0.5));
                    angle += shape->parent->transforms.rotation;
                }

                // Rotate around the shape's own center in its dst rect
                SDL_Point pivot { dst.w / 2, dst.h / 2 };
                SDL_RenderCopyEx(renderer, shape->cached, nullptr, &dst, angle, &pivot, SDL_FLIP_NONE);
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