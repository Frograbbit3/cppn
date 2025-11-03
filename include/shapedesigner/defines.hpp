#pragma once
#include <string>
#include "../graphics/graphics_core.hpp"
#include "../graphics/graphics_utils.hpp"
// Forward declarations to avoid include-order issues
struct SDL_Texture; // from SDL2
namespace CPPN { namespace ShapeDesigner { struct Shape; SDL_Texture* ConvertShapeToTexture(Shape* shape); } }
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

            std::vector<Vector2> points; // Optional list of points for polygons

            Shape* parent = nullptr; //only used if you want subgrouping
            ShapeTypes shape = ShapeTypes::RECTANGLE; 
            SDL_Texture* cached = nullptr; // used internally for rendering
            SDL_Rect* cached_rect = nullptr; //used internally for rendering
            void cache() {
                this->cached = ConvertShapeToTexture(this);
                this->cached_rect = new SDL_Rect{this->position.x, this->position.y, this->size.width, this->size.height};
            }
        };
    }
}