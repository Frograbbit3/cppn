#pragma once
#include <string>
#include "../graphics/graphics_core.hpp"
#include "../graphics/graphics_utils.hpp"
using CPPN::Graphics::Color;
SDL_PixelFormat* PIXEL_FORMAT = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
const uint32_t TRANSPARENT_COLOR = Color(0,0,0,0).pack(PIXEL_FORMAT);
namespace CPPN {
    namespace ShapeDesigner {
        struct Vector2 {
            int x=0;
            int y=0;
        };
        struct Size2 {
            int width=50;
            int height=50;
            constexpr int radius() const noexcept {
                return (width + height) / 4;
            }
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
    }
}