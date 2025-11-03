#include <SDL2/SDL.h>
#include "equations.hpp"
#include "defines.hpp"
#include "filesystem/filesystem_core.hpp"
#include <SDL2/SDL_ttf.h>

namespace {
    // Lazy-loaded shared font opened from the app's resource path
    TTF_Font* GetSharedFont() {
        static TTF_Font* s_font = nullptr;
        if (!s_font) {
            std::string path = CPPN::FileSystem::AbsoluteResourcePath("Roboto-Regular.ttf");
            s_font = TTF_OpenFont(path.c_str(), 32);
            if (!s_font) {
                SDL_Log("TTF_OpenFont failed for %s: %s", path.c_str(), TTF_GetError());
            }
        }
        return s_font;
    }
}

namespace CPPN {
    namespace ShapeDesigner {
        SDL_Texture* GenerateOval(Shape* shape) {
            int width = shape->size.width;
            int height = shape->size.height;
            Uint32* pixels = (Uint32*)malloc(width * height * sizeof(Uint32));
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    pixels[y * width + x] = Math::IsInOval(x, y, width, height) ? shape->fillColor.pack(PIXEL_FORMAT) : TRANSPARENT_COLOR;
                }
            }
            return ConvertPixelMapToTexture(pixels, width, height);
        }
        SDL_Texture* GeneratePolygon(Shape* shape) {
            if (shape->points.empty()) {throw std::runtime_error("Please provide a points list!");}
            int minX = shape->points[0].x, maxX = shape->points[0].x;
            int minY = shape->points[0].y, maxY = shape->points[0].y;
            for (const auto& pt : shape->points) {
                if (pt.x < minX) minX = pt.x;
                if (pt.x > maxX) maxX = pt.x;
                if (pt.y < minY) minY = pt.y;
                if (pt.y > maxY) maxY = pt.y;
            }
            int polyWidth = maxX - minX;
            int polyHeight = maxY - minY;
            int offsetX = (shape->size.width - polyWidth) / 2 - minX;
            int offsetY = (shape->size.height - polyHeight) / 2 - minY;
            int width = shape->size.width;
            int height = shape->size.height;
            Uint32* pixels = (Uint32*)malloc(width * height * sizeof(Uint32));
            
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    pixels[y * width + x] = Math::IsInPolygon(x - offsetX, y - offsetY, shape->points) ? shape->fillColor.pack(PIXEL_FORMAT) : TRANSPARENT_COLOR;
                }
            }
            return ConvertPixelMapToTexture(pixels, width, height);
        }

        SDL_Texture* GenerateLabel(Shape* shape) {
            TTF_Font* font = GetSharedFont();
            if (!font) {
                SDL_Log("GenerateLabel: Font not available");
                return nullptr;
            }
            
            if (shape->value.empty()) {
                SDL_Log("GenerateLabel: value is empty");
                return nullptr;
            }
            
            SDL_Color color = {shape->fillColor.red, shape->fillColor.green, shape->fillColor.blue, shape->fillColor.alpha};
            SDL_Surface* surface = TTF_RenderText_Blended(font, shape->value.c_str(), color);
            if (!surface) {
                SDL_Log("TTF_RenderText_Blended failed: %s", TTF_GetError());
                return nullptr;
            }
            
            // Update shape size to match rendered text dimensions
            shape->size.width = surface->w;
            shape->size.height = surface->h;
            
            SDL_Texture* texture = SDL_CreateTextureFromSurface(CPPN::Graphics::renderer, surface);
            SDL_FreeSurface(surface);
            
            if (!texture) {
                SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
            }
            
            return texture;
        }

        SDL_Texture* GenerateCircle(Shape* shape) {
            const int radius = shape->size.radius();
            const int di = (radius*2);
            Uint32* pixels = (Uint32*)malloc(di * di * sizeof(Uint32));
            for (int y = 0; y < di; y++) {
                for (int x = 0; x < di; x++) {
                    pixels[y * di + x] = Math::IsInCircle(x, y, radius) ? shape->fillColor.pack(PIXEL_FORMAT) : TRANSPARENT_COLOR;
                }
            }
            return ConvertPixelMapToTexture(pixels, di, di);
        }

        
        SDL_Texture* GenerateRect(Shape* shape) {
            int width = shape->size.width;
            int height = shape->size.height;
            Uint32* pixels = (Uint32*)malloc(width * height * sizeof(Uint32));
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    pixels[y * width + x] = shape->fillColor.pack(PIXEL_FORMAT);
                }
            }
            return ConvertPixelMapToTexture(pixels, width, height);
        }


    }
}