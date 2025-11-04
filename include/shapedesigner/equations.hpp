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
            // Set blend mode to support alpha transparency
            if (texture) {
                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            }
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

        constexpr inline CPPN::ShapeDesigner::Vector2 RotatePoint(
            const CPPN::ShapeDesigner::Vector2& point,
            const CPPN::ShapeDesigner::Vector2& origin,
            double angle) {
            const double s = std::sin(angle);
            const double c = std::cos(angle);
            double x = static_cast<double>(point.x) - static_cast<double>(origin.x);
            double y = static_cast<double>(point.y) - static_cast<double>(origin.y);
            double xnew = x * c - y * s;
            double ynew = x * s + y * c;
            return { static_cast<int>(std::round(xnew + origin.x)), static_cast<int>(std::round(ynew + origin.y)) };
        }
        constexpr inline CPPN::ShapeDesigner::Vector2 ComputeWorldCenter(
            const CPPN::ShapeDesigner::Shape* shape) {
            if (!shape->parent) {
                return {
                    static_cast<int>(std::round(shape->position.x + shape->size.width * 0.5)),
                    static_cast<int>(std::round(shape->position.y + shape->size.height * 0.5))
                };
            }

            // parent center (world)
            const double parentCx = static_cast<double>(shape->parent->position.x) + shape->parent->size.width * 0.5;
            const double parentCy = static_cast<double>(shape->parent->position.y) + shape->parent->size.height * 0.5;

            // child local center relative to parent's local space
            const double childLocalCx = static_cast<double>(shape->position.x) + shape->size.width * 0.5;
            const double childLocalCy = static_cast<double>(shape->position.y) + shape->size.height * 0.5;

            // offset from parent's center
            double vx = childLocalCx - shape->parent->size.width * 0.5;
            double vy = childLocalCy - shape->parent->size.height * 0.5;

            // rotate by parent's rotation
            const double rad = shape->parent->transforms.rotation * M_PI / 180.0;
            const double cs = std::cos(rad);
            const double sn = std::sin(rad);
            const double rx = cs * vx - sn * vy;
            const double ry = sn * vx + cs * vy;

            // combine
            return {
                static_cast<int>(std::round(parentCx + rx)),
                static_cast<int>(std::round(parentCy + ry))
            };
        }

    }
}

