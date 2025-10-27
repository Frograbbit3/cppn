#pragma once
#include "SDL2/SDL.h"
#include "graphics_utils.hpp"
namespace CPPN { namespace Graphics { void AddShape(class BaseShape*); } }
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
            bool draggable = false;
            virtual ~BaseShape() = default;
            const Color &getColor() const noexcept { return color; }
            void setColor(const Color &c) noexcept { color = c; }
            void setPosition(const int &xpos, const int &ypos) {x=xpos;y=ypos;}
            virtual void draw(SDL_Renderer *ren) const = 0;
            virtual void update() {}
            virtual bool isColliding(int x, int y) const {return false;}

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
            bool isColliding(int x, int y) const override {
                return (rect.x <= x && x <= rect.x + rect.w) && (rect.y <= y && y <= rect.y + rect.h);
            }
        };
    } // namespace Graphics

}