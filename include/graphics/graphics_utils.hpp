#pragma once
#include <cstdint>
#include "SDL2/SDL.h"
namespace CPPN {
    namespace Graphics {
        struct Color {
            uint8_t red=255;
            uint8_t green=255;
            uint8_t blue=255;
            uint8_t alpha=255;
            constexpr Color() noexcept = default;
            constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
                : red(r), green(g), blue(b), alpha(a) {}
            inline Uint32 pack(SDL_PixelFormat *fmt) const {
                return SDL_MapRGBA(fmt, red, green, blue, alpha);
            }
        };
    }
}