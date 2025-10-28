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
            BaseShape(int x = 0, int y = 0, CPPN::Graphics::Color color = {0,0,0, 255})
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

       } // namespace Graphics

}