#pragma once
#include "graphics_general.hpp"
#include "graphics_core.hpp"
#include "graphics_svg.hpp"
#include "graphics_utils.hpp"
namespace CPPN
{
    namespace Graphics
    {
        class Rectangle : public BaseShape
        {
        private:
            SDL_Rect rect;
            SDL_Texture *texture;
            CPPN::Graphics::RectangleProperties properties;

        public:

            /*
                Creates a rectangle.

                @param x The x position.
                @param y The y position.
                @param width The width of the rectangle. Pass in 0 to use the width from the RectangleProperties
                @param height The height of the rectangle. Pass in 0 to use the height from the RectangleProperties
                @param settings Design for the rectangle. Created using a CPPN::Graphics::RectangleProperties.
            */
            Rectangle(int x, int y, int width, int height, CPPN::Graphics::RectangleProperties settings)
                : BaseShape(x, y, settings.fill), properties(settings)
            {
                if (width > 1) {
                    properties.width = width;
                }             
                if (height > 1) {
                    properties.height = height;
                }
                update(true);
            }


            /*
                Updates the texture.

                @param full Rerenders the texture too.
            */
            virtual void update(bool full) override
            {
                rect.x = x;
                rect.y = y;
                rect.w = properties.width+ properties.strokeWidth;
                rect.h = properties.height+ properties.strokeWidth;
                if (full) {
                    texture = CPPN::Graphics::LoadSVG(properties.generateSVG(), CPPN::Graphics::renderer);
                }
            }

            /*
                Blits the rectangle to the screen. 
                @param ren Use CPPN::Graphics::renderer.
            */
            void draw(SDL_Renderer *ren) const override
            {
                SDL_RenderCopyEx(ren, texture, nullptr, &rect, rotation, nullptr, SDL_FLIP_NONE);
            }
            /*
                Does AABB collision to check for collision in the rect.
                @param px The point's x.
                @param py The point's y.
            */
            bool isColliding(int px, int py) const override
            {
                return (px >= x && px <= x + properties.width) &&
                    (py >= y && py <= y + properties.height);
            }
            /*
                Checks if this shape is colliding with another shape.

                @param shape The shape to collide against
                @param steps The amount of collision steps. Higher is better but slower.
                @note Rotation is not currently supported. This is also pixel perfect, assuming no rotation.
            */
            bool isCollidingShape(BaseShape &shape, int steps = 8) const override {
                for (int xP = x; xP < x + properties.width; xP += steps) {
                    for (int yP = y; yP < y + properties.height; yP += steps) {
                        if (shape.isColliding(xP, yP)) {
                            return true;
                        }
                    }
                }
                return false;
            }
        };
        class Oval : public BaseShape
        {
        private:
            SDL_Texture *texture;
            SDL_Rect rect;

        public:
            CPPN::Graphics::OvalProperties properties;
            /*
                Creates an oval.

                @param x The x position.
                @param y The y position.
                @param settings Design for the oval. Created using a CPPN::Graphics::OvalProperties.
            */
            Oval(int x, int y, CPPN::Graphics::OvalProperties settings)
                : BaseShape(x, y, properties.fill), properties(settings)
            {
               // texture = CPPN::Graphics::LoadSVG(properties.generateSVG(), CPPN::Graphics::renderer);
                update(true);
            }


            /*
                Updates the properties.
                @param full Rerenders the texture too.
            */
            virtual void update(bool full) override
            {
                rect.x = x;
                rect.y = y;
                rect.h = properties.height+ properties.strokeWidth*2;
                rect.w = properties.width + properties.strokeWidth*2;
                if (full) {
                    texture = CPPN::Graphics::LoadSVG(properties.generateSVG(), CPPN::Graphics::renderer);
                }
            }


            /*
                Blits the oval to the screen.
                @param ren Use CPPN::Graphics::renderer
            */
            void draw(SDL_Renderer *ren) const override
            {
                SDL_RenderCopyEx(ren, texture, nullptr, &rect, rotation, nullptr, SDL_FLIP_NONE);
            }


            /*
                Does oval based collision.
                @param px Point's x.
                @param py Point's y.
            */
            bool isColliding(int px, int py) const override {
                int cx = rect.x + rect.w / 2;
                int cy = rect.y + rect.h / 2;
                int a = rect.w / 2;
                int b = rect.h / 2;
                double dx = (px - cx) / double(a);
                double dy = (py - cy) / double(b);
                return dx*dx + dy*dy <= 1.0;
            }
            /*
                Checks if this shape is colliding with another shape.

                @param shape The shape to collide against
                @param steps The amount of collision steps. Higher is better but slower.
                @note Rotation is not currently supported. This is also pixel perfect, assuming no rotation.
            */
            bool isCollidingShape(BaseShape &shape, int steps = 8) const override {
                for (int xP = x; xP < x + properties.width; xP += steps) {
                    for (int yP = y; yP < y + properties.height; yP += steps) {
                        if (shape.isColliding(xP, yP)) {
                            return true;
                        }
                    }
                }
                return false;
            }
        };

    }
} // namespace CPPN
