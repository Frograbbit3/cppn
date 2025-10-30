#include <SDL2/SDL.h>
#include "equations.hpp"
#include "defines.hpp"

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