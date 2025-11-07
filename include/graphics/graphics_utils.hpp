#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "backend/backend.h"
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

        uint32_t CompactBinary(const int bin[31]) {
            uint32_t m;
            for (int i = 0; i < 31; i++) {
                m |= (bin[i] << (31-i));
            }
            return m;
        }

        /*
            Produce a simple 2D grayscale mask (0 or 1) from an SDL_Texture.
            Uses SDL_RenderReadPixels to read the texture's pixel data.
            Returns a vector of rows, each row is a vector<uint8_t> of length width.
        */
        std::vector<std::vector<uint8_t>> GetGrayscale(SDL_Renderer* renderer, SDL_Texture* texture) {
            if (!renderer) {
                std::cerr << "GetGrayscale: Renderer is null.\n";
                return {};
            }
            if (!texture) {
                std::cerr << "GetGrayscale: Texture is null.\n";
                return {};
            }

            int w = 0, h = 0;
            if (SDL_QueryTexture(texture, nullptr, nullptr, &w, &h) != 0 || w <= 0 || h <= 0) {
                std::cerr << "GetGrayscale: Failed to query texture dimensions.\n";
                return {};
            }

            SDL_Texture* tempTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);
            if (!tempTexture) {
                std::cerr << "GetGrayscale: Failed to create temporary render-target texture.\n";
                return {};
            }

            SDL_SetTextureBlendMode(tempTexture, SDL_BLENDMODE_NONE);
            SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer);
            SDL_SetRenderTarget(renderer, tempTexture);

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);

            std::vector<unsigned char> pixels((size_t)w * h * 4);
            if (SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA32, pixels.data(), w * 4) != 0) {
                std::cerr << "GetGrayscale: SDL_RenderReadPixels failed: " << SDL_GetError() << "\n";
                SDL_SetRenderTarget(renderer, prevTarget);
                SDL_DestroyTexture(tempTexture);
                return {};
            }

            SDL_SetRenderTarget(renderer, prevTarget);
            SDL_DestroyTexture(tempTexture);

            std::vector<std::vector<uint8_t>> mask;
            mask.assign(h, std::vector<uint8_t>(w, 0));

            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    size_t idx = ((size_t)y * (size_t)w + (size_t)x) * 4;
                    unsigned char r = pixels[idx + 0];
                    unsigned char g = pixels[idx + 1];
                    unsigned char b = pixels[idx + 2];
                    unsigned char a = pixels[idx + 3];
                    int avg = (int(r) + int(g) + int(b) + int(a)) / 4;
                    mask[y][x] = (avg > 0) ? 1 : 0;
                }
            }

            std::cerr << "GetGrayscale: Successfully generated mask.\n";
            return mask;
        }
    
    }
}