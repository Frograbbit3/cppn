#include <SDL2/SDL.h>
#include "equations.hpp"
#include "defines.hpp"
#include "filesystem/filesystem_core.hpp"
#include "../ttfs/text.hpp"
CPPN::FontSystem::Font* font = nullptr;
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
            
            
            CPPN::FileSystem::Init("ex","ex");
            if (font == nullptr) {
                 font = new CPPN::FontSystem::Font{CPPN::FileSystem::AbsoluteResourcePath("Roboto-Regular.ttf"), 32.0f}; // temp.
            }


            if (shape->value.empty()) {
                SDL_Log("GenerateLabel: value is empty");
                return nullptr;
            }
            
            SDL_Color color = {shape->fillColor.red, shape->fillColor.green, shape->fillColor.blue, shape->fillColor.alpha};
            // Get bitmap data from font system
            unsigned char* bitmap = font->GetTextAsBitmap(shape->value);
            if (!bitmap) {
                SDL_Log("GetTextAsBitmap failed");
                return nullptr;
            }

            // Assume font.GetTextAsBitmap also provides width and height, or you need to get them from font or shape
            int textWidth = font->textWidth;
            int textHeight = font->textHeight;
            Uint32* pixels = (Uint32*)malloc(textWidth * textHeight * sizeof(Uint32));
            
            // Allocate format once outside the loop
            SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
            
            for (int i = 0; i < textWidth * textHeight; i++) {
                unsigned char alpha = bitmap[i];
                Uint32 pixel = SDL_MapRGBA(format,
                                           shape->fillColor.red,
                                           shape->fillColor.green,
                                           shape->fillColor.blue,
                                           alpha);
                pixels[i] = pixel;
            }
            
            // Free the format after use
            SDL_FreeFormat(format);
            free(bitmap);

            // Update shape size to match rendered text dimensions
            shape->size.width = textWidth;
            shape->size.height = textHeight;

            SDL_Texture* texture = ConvertPixelMapToTexture(pixels, textWidth, textHeight);
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