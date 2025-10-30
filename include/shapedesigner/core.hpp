#pragma once
#include "../graphics/graphics_core.hpp"
#include "../graphics/graphics_utils.hpp" //purely for the color class
#include <SDL2/SDL.h>


using CPPN::Graphics::Color;
namespace CPPN {
    namespace ShapeDesigner {
        struct Vector2 {
            int x=0;
            int y=0;
        };
        struct Size2 {
            int width=50;
            int height=50;
        };

        struct Transforms {
            int rotation;
        };

        enum class ShapeTypes {
            RECTANGLE=0,
            CIRCLE=1,
            OVAL=2,
            LINE=3,
            POLYLINE=4,
            POLYGON=5
        };


        //TODO Add font suport
        enum class Font {}; 
        struct Text {
            std::string value;
            Font font;  
        };

        struct Shape {
            Color fillColor;
            Color strokeColor;
            int strokeWidth;

            Vector2 position;
            Size2 size;
            Transforms transforms;

            Shape* parent = nullptr; //only used if you want subgrouping
            ShapeTypes shape = ShapeTypes::RECTANGLE; 
        };

        SDL_Texture* ConvertPixelMapToTexture(Uint32* pixels, int width, int height) {
            SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
                pixels, width, height, 32, width * 4,
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
            );
            SDL_Texture* texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
            SDL_FreeSurface(surface);
            return texture;
        }

        SDL_Texture* GenerateRect(Shape* shape) {
            int width = shape->size.width;
            int height = shape->size.height;
            Uint32* pixels = (Uint32*)malloc(width * height * sizeof(Uint32));
            SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    pixels[y * width + x] = shape->fillColor.pack(format);
                    SDL_FreeFormat(format);
                }
            }
            return ConvertPixelMapToTexture(pixels, width, height);
        }

        SDL_Texture* ConvertShapeToTexture(Shape* shape) {
            switch (shape->shape) {
                case (ShapeTypes::RECTANGLE):
                    return GenerateRect(shape);
                    break;
            }
            return nullptr;
        }

    }
}
