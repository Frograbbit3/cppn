#pragma once
#include <string>
#include <vector>
#include <algorithm>
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
            int rotation = 0;
        };

        enum class ShapeTypes {
            RECTANGLE=0,
            CIRCLE=1,
            OVAL=2,
            LINE=3,
            POLYLINE=4,
            POLYGON=5,
            LABEL=6
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
            std::string value=""; //currently used for labels

            SDL_Texture* cached = nullptr; // used internally for rendering
            SDL_Rect* cached_rect = nullptr; //used internally for rendering
            // Auto-register on construction; auto-deregister on destruction
            Shape() {
                CPPN::Graphics::AddShape(this);
            }
            ~Shape() {
                CPPN::Graphics::RemoveShape(this);
                if (cached) { SDL_DestroyTexture(cached); cached = nullptr; }
                if (cached_rect) { delete cached_rect; cached_rect = nullptr; }
            }
            void cache() {
                if (this->cached) { SDL_DestroyTexture(this->cached); this->cached = nullptr; }
                if (!this->cached_rect) this->cached_rect = new SDL_Rect{};
                this->cached = ConvertShapeToTexture(this);
                this->cached_rect->x = this->position.x;
                this->cached_rect->y = this->position.y;
                this->cached_rect->w = this->size.width;
                this->cached_rect->h = this->size.height;
            }

            bool IsCollidingShape(Shape* other, int step = 8) {
                if (!other) return false;
                if (this == other) return true;

                // AABB early-out in world space
                const int ax1 = this->position.x;
                const int ay1 = this->position.y;
                const int ax2 = ax1 + this->size.width;
                const int ay2 = ay1 + this->size.height;

                const int bx1 = other->position.x;
                const int by1 = other->position.y;
                const int bx2 = bx1 + other->size.width;
                const int by2 = by1 + other->size.height;

                if (ax2 <= bx1 || bx2 <= ax1 || ay2 <= by1 || by2 <= ay1) return false;

                // Scan only the overlapping region
                step = std::max(1, step);
                const int sx = std::max(ax1, bx1);
                const int ex = std::min(ax2, bx2);
                const int sy = std::max(ay1, by1);
                const int ey = std::min(ay2, by2);

                for (int y = sy; y < ey; y += step) {
                    for (int x = sx; x < ex; x += step) {
                        if (this->IsColliding(x, y) && other->IsColliding(x, y)) {
                            return true;
                        }
                    }
                }

                // Last-chance check at the bottom-right edge if step doesn't divide evenly
                if (this->IsColliding(ex - 1, ey - 1) && other->IsColliding(ex - 1, ey - 1)) return true;
                return false;
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
                    //just does AABB it sucks lol
                    case ShapeTypes::LABEL:
                        return true;
                        
                    default:
                        return false;
                }
            }
         };
    }
}