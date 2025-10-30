#pragma once
#include <cmath>
#include <SDL2/SDL.h>
#include "defines.hpp"
using namespace CPPN;

namespace CPPN {
    namespace ShapeDesigner {
        SDL_Texture* ConvertPixelMapToTexture(Uint32* pixels, int width, int height) {
            SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
                pixels, width, height, 32, width * 4,
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
            );
            SDL_Texture* texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
            SDL_FreeSurface(surface);
            return texture;
        }
    }
    namespace Math{
        constexpr inline int pow(int num, int pow) noexcept {
            int new_num=1;
            for (int i = 0; i < pow; i++) {
                new_num *= num;
            }
            return new_num;
        }
        /*The quake algorithm. I had to, i'm sorry.*/
        constexpr inline float sqrt(float number) noexcept {
            long i=0;
            float x2=0.0f;
            float y=0.0f;
            const float threehalfs = 1.5F;

            x2 = number * 0.5F;
            y = number;
            i = *(long *) &y;               // Evil floating point bit level hacking
            i = 0x5f3759df - (i >> 1);      // What the f***?
            y = *(float *) &i;
            y = y * (threehalfs - (x2 * y * y)); // 1st iteration
            return number*y;
        }

        /*Not really needed but it completes the set*/
        constexpr inline bool IsInRect(int x, int y, int width, int height) noexcept {
            return (x >= 0 && x <=width) && (y>=0 && y<=height);
        };

        constexpr inline bool IsInCircle(int x, int y, int radius) noexcept {
            return (x*x + y*y < radius*2);
        };

        constexpr inline bool IsInOval(int x, int y, int width, int height) noexcept {
            const int a = width  / 2;
            const int b = height / 2;

            const int dx = x - a;
            const int dy = y - b;

            return (dx * dx * b * b + dy * dy * a * a) <= (a * a * b * b);
        }


    }
}

