#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "SDL2/SDL.h"
#define NANOSVG_IMPLEMENTATION	// Expands implementation
#include "../nanosvg/nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "../nanosvg/nanosvgrast.h"
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
            /*Converts this object to an SDL_MapRGBA.*/
            inline Uint32 pack(SDL_PixelFormat *fmt) const {
                return SDL_MapRGBA(fmt, red, green, blue, alpha);
            }
        };
        /*
            Converts an SVG to an SDL_Texture. Private use.
            @param svgData The string of SVG you want to render.
            @param ren CPPN::Graphics::renderer.

            @return An SDL texture struct.
        */
        SDL_Texture* LoadSVG(const std::string& svgData, SDL_Renderer* ren) {
            // Parse the SVG string
            NSVGimage* image = nsvgParse((char*)svgData.c_str(), "px", 96);
            if (!image) {
                printf("SVG parse failed\n");
                return nullptr;
            }

            int w = (int)image->width;
            int h = (int)image->height;

            // Allocate pixel buffer (RGBA)
            std::vector<unsigned char> pixels(w * h * 4);

            // Rasterize into buffer
            NSVGrasterizer* rast = nsvgCreateRasterizer();
            nsvgRasterize(rast, image, 0, 0, 1.0f, pixels.data(), w, h, w * 4);

            // Create SDL surface & texture
            SDL_Surface* surf = SDL_CreateRGBSurfaceFrom(
                pixels.data(), w, h, 32, w * 4,
                0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
            );

            SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
            SDL_FreeSurface(surf);

            nsvgDeleteRasterizer(rast);
            nsvgDelete(image);

            return tex;
        }
    }
}