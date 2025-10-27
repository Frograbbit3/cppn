#pragma once
#include "SDL2/SDL.h"
#include "graphics_utils.hpp"

// forward-declare AddShape so BaseShape can auto-register without including
// graphics_core.hpp (avoid circular include). The function is defined in
// graphics_core.hpp and takes ownership/management there.
namespace CPPN { namespace Graphics { void AddShape(class BaseShape*); } }

// Forward-declare AddShape so BaseShape can auto-register itself without
// including graphics_core.hpp (avoids circular include).
namespace CPPN { namespace Graphics { class BaseShape; void AddShape(BaseShape*); } }
namespace CPPN
{
    namespace Graphics
    {

        class BaseShape
        {
        protected:
            CPPN::Graphics::Color color;
            BaseShape(int x = 0, int y = 0, CPPN::Graphics::Color color = {255, 255, 255, 255})
                : x(x), y(y), color(color) {
                CPPN::Graphics::AddShape(this);
            }

        public:
            int x, y;
            virtual ~BaseShape() = default;
            const Color &getColor() const noexcept { return color; }
            void setColor(const Color &c) noexcept { color = c; }
            void setPosition(const int &xpos, const int &ypos) {x=xpos;y=ypos;}
            virtual void draw(SDL_Renderer *ren) const = 0;
            virtual void update() {}

        };

        class Rectangle : public BaseShape
        {
        private:
            SDL_Rect rect;

        public:
            int width;
            int height;
            Rectangle(int x, int y, int wid, int hei, CPPN::Graphics::Color color = {255, 255, 255, 255})
                : BaseShape(x, y, color), width(wid), height(hei)
            {
                rect.x = this->x;
                rect.y = this->y;
                rect.w = width;
                rect.h = height;
            }

            virtual void update() override
            {
                rect.x = static_cast<int>(x);
                rect.y = static_cast<int>(y);
                rect.w = width;
                rect.h = height;
            }
            void draw(SDL_Renderer *ren) const override
            {
                SDL_RenderFillRect(ren, &rect);
            }
        };
    } // namespace Graphics

}