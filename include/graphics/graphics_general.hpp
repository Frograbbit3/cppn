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
            double rotation=0;
            bool draggable = false;
            virtual ~BaseShape() = default;
            /*
                Gets the color of the shape. Some shapes (such as Images) may not have a color.
                @return Returns a CPPN::Graphics::Color.
            */
            const Color &getColor() const noexcept { return color; }
            /*
                @deprecated Sets the color of a shape. Modifying the properties, if provided, is preferred.
                @param c Takes in a CPPN::Graphics::Color.
            */
            void setColor(const Color &c) noexcept { color = c; }
             /*
                Sets the position of a shape. 
                @param xpos the X position.
                @param ypos the Y position.
            */
            void setPosition(const int &xpos, const int &ypos) {x=xpos;y=ypos;}
            /*
                Designed for internal use. Can render a shape directly to the screen, but is not recommended (it can break Z axis).
                @param ren an SDL_Renderer. You can get this from CPPN::Graphics::renderer.
            */
            virtual void draw(SDL_Renderer *ren) const = 0;
            /*
                Will update shapes to fix property changes.
            */
            virtual void update() {}
            /*
                Checks if a point is within the boundaries of the shape.
                @param x The x position to check.
                @param y The y position to check.
                @return Returns if it collides.
            */
            virtual bool isColliding(int x, int y) const {return false;}

        };

       } // namespace Graphics

}