#pragma once
#include <cmath>
#include <SDL2/SDL.h>
#include "defines.hpp"
#include <fmt/core.h>
#include "../ttfs/stb_truetype.h"
// Forward-declare the renderer symbol to break include order cycles
struct SDL_Renderer; // from SDL2
namespace CPPN { namespace Graphics { extern SDL_Renderer* renderer; } }
using namespace CPPN;
using namespace CPPN::ShapeDesigner;
namespace CPPN {
    namespace ShapeDesigner {
        SDL_Texture* ConvertPixelMapToTexture(Uint32* pixels, int width, int height) {
            SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
                pixels, width, height, 32, width * 4,
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
            );
            SDL_Texture* texture = SDL_CreateTextureFromSurface(CPPN::Graphics::renderer, surface);
            SDL_FreeSurface(surface);
            // pixels was allocated by caller with malloc; safe to free after texture creation
            if (pixels) { free(pixels); }
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
        constexpr float sqrt(float x) noexcept {
            if (x <= 0.0f) return 0.0f;
            float guess = x;
            for (int i = 0; i < 8; ++i)
                guess = 0.5f * (guess + x / guess);
            return guess;
        }

        /*Not really needed but it completes the set*/
        constexpr inline bool IsInRect(int x, int y, int width, int height) noexcept {
            return (x >= 0 && x <=width) && (y>=0 && y<=height);
        }

        // For relative points (relative to shape's position)
        bool IsInPolygon(int x, int y, const std::vector<Vector2>& pts) noexcept {
            bool inside = false;
            for (size_t i = 0, j = pts.size() - 1; i < pts.size(); j = i++) {
                bool intersect = ((pts[i].y > y) != (pts[j].y > y)) &&
                                (x < (pts[j].x - pts[i].x) * (y - pts[i].y) / float(pts[j].y - pts[i].y) + pts[i].x);
                if (intersect) inside = !inside;
            }
            return inside;
        }

        constexpr inline bool IsInCircle(int x, int y, int radius) noexcept {
            int dx = x - radius;
            int dy = y - radius;
            return (dx * dx + dy * dy) <= (radius * radius);
        }

        constexpr inline bool IsInOval(int x, int y, int width, int height) noexcept {
            const int a = width  / 2;
            const int b = height / 2;

            const int dx = x - a;
            const int dy = y - b;

            return (dx * dx * b * b + dy * dy * a * a) <= (a * a * b * b);
        }


    }
}

