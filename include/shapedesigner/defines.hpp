#pragma once
#include <string>
#include <vector>
#include "../graphics/graphics_utils.hpp"
#include "equations.hpp"
// Forward declarations to avoid include-order issues
struct SDL_Texture; // from SDL2
namespace CPPN { namespace ShapeDesigner { struct Vector2; struct Shape; SDL_Texture* ConvertShapeToTexture(Shape* shape); } }
namespace CPPN { namespace Graphics { void AddShape(CPPN::ShapeDesigner::Shape* shape); void RemoveShape(CPPN::ShapeDesigner::Shape* shape); } }
// Forward declare Math predicates that depend on Vector2
namespace CPPN { namespace Math {
    constexpr inline bool IsInRect(int x, int y, int width, int height) noexcept;
    bool IsInPolygon(int x, int y, const std::vector<CPPN::ShapeDesigner::Vector2>& pts) noexcept;
    constexpr inline bool IsInCircle(int x, int y, int radius) noexcept;
    constexpr inline bool IsInOval(int x, int y, int width, int height) noexcept;
} }
using CPPN::Graphicss::Color;
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
            bool draggable = false;

            SDL_Texture* cached = nullptr; // used internally for rendering
            SDL_Rect* cached_rect = nullptr; //used internally for rendering
            // Auto-register on construction; auto-deregister on destruction
            Shape() {
                CPPN::Graphics::AddShape(this);
            }
            ~Shape() {
                CPPN::Graphics::RemoveShape(this);
                if (cached_rect) { delete cached_rect; cached_rect = nullptr; }
                // cached texture is owned by renderer; not destroyed here
            }
            void cache() {
                this->cached = ConvertShapeToTexture(this);
                this->cached_rect = new SDL_Rect{this->position.x, this->position.y, this->size.width, this->size.height};
            }

            bool IsColliding(int x, int y) {
                if (this->size.width <= 0 || this->size.height <= 0) return false;

                // Convert point to local coordinates and AABB check via Math helper
                const int lx = x - this->position.x;
                const int ly = y - this->position.y;
                if (!CPPN::Math::IsInRect(lx, ly, this->size.width, this->size.height)) return false;

                switch (this->shape) {
                    case ShapeTypes::RECTANGLE:
                        return true; // AABB hit
                    case ShapeTypes::CIRCLE:
                        return CPPN::Math::IsInCircle(lx, ly, this->size.radius());
                    case ShapeTypes::OVAL:
                        return CPPN::Math::IsInOval(lx, ly, this->size.width, this->size.height);
                    case ShapeTypes::POLYGON:
                        if (this->points.empty()) return false;
                        return CPPN::Math::IsInPolygon(lx, ly, this->points);
                    case ShapeTypes::LINE:
                        return false;
                    case ShapeTypes::POLYLINE:
                        // TODO: implement distance-to-segment hit test
                        return false;
                    default:
                        return false;
                }
            }
         };
    }
}